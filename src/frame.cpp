// Prints the game frame to terminal

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstdio> 
#include <thread>
#include "frame.h"

frame::frame(void){

    // Create a text file with the current empty frame
    try{
        frameFileOut.open(filename);
        frameFileIn.open(filename);

        // Check if the file was created / opened
        if ((!frameFileOut.is_open()) || (!frameFileIn.is_open())) {
            throw(std::runtime_error("Unable to open file!"));
        }
    }catch(const std::runtime_error& e){
        std::cerr << "Runtime error caught: " << e.what() << std::endl;
    }
    
    std::cout << '\r' << std::endl;

    // Set the frame render lock flag to true
    f_renderLockAcquired = true;

    clearFrameWorker();
}

frame::frame(char border):borderChar(border){

    // Create a text file with the current empty frame
    try{
        frameFileOut.open(filename);
        frameFileIn.open(filename);

        // Check if the file was created / opened
        if ((!frameFileOut.is_open()) || (!frameFileIn.is_open())) {
            throw(std::runtime_error("Unable to open file!"));
        }
    }catch(const std::runtime_error& e){
        std::cerr << "Runtime error caught: " << e.what() << std::endl;
    }

    // Set the border flag to true
    f_setBorder = true;
    
    // Set the frame render lock flag to true
    f_renderLockAcquired = true;

    clearFrameWorker();
}

frame::frame(fps fps){

    // Create a text file with the current empty frame
    try{
        frameFileOut.open(filename);
        frameFileIn.open(filename);

        // Check if the file was created / opened
        if ((!frameFileOut.is_open()) || (!frameFileIn.is_open())) {
            throw(std::runtime_error("Unable to open file!"));
        }
    }catch(const std::runtime_error& e){
        std::cerr << "Runtime error caught: " << e.what() << std::endl;
    }
    
    std::cout << '\r' << std::endl;
    clearFrameWorker();

    // Set class fps variable
    updateFPS(fps);

    // Initialise the mutex. Do not lock
    std::unique_lock<std::mutex> lock(renderMutex, std::defer_lock);

    // Start the frame render worker thread
    frameRenderThread = std::thread(&frame::frameRenderWorker, this);
    
}

frame::frame(fps fps, char border):borderChar(border){

    // Create a text file with the current empty frame
    try{
        frameFileOut.open(filename);
        frameFileIn.open(filename);

        // Check if the file was created / opened
        if ((!frameFileOut.is_open()) || (!frameFileIn.is_open())) {
            throw(std::runtime_error("Unable to open file!"));
        }
    }catch(const std::runtime_error& e){
        std::cerr << "Runtime error caught: " << e.what() << std::endl;
    }

    // Set the border flag to true
    f_setBorder = true;

    // Set the frame size variable
    getBorderedFrameSize();
    
    clearFrameWorker();

    // Set class fps variable
    updateFPS(fps);

    // Initialise the mutex. Do not lock
    std::unique_lock<std::mutex> lock(renderMutex, std::defer_lock);

    // Start the frame render thread
    frameRenderThread = std::thread(&frame::frameRenderWorker, this);
}

frame::~frame(void){

    // Close pointer to temp file
    frameFileOut.close();
    frameFileIn.close();

    // Close thread
    if(f_renderActive == true){
        if(frameRenderThread.joinable() == true){
            f_renderActive = false;     // Trigger the thread to return

            frameRenderThread.join();
        }
    }

    // delete file
    try {
        int result = remove(const_cast<char*>(filename.c_str()));
        if (result != 0) {
            // If remove() returns non-zero, throw an exception
            throw std::runtime_error("Failed to delete file."); 
        }
    } catch (const std::runtime_error& e) {
        std::cerr << "Runtime error caught: " << e.what() << std::endl; 
    }

}

void frame::frameRenderWorker(void){
    // Set the renderer active flag
    f_renderActive = true;

    // DEBUG_PRINT ("lines %d\n", termSize.rows);
    // DEBUG_PRINT ("columns %d\n", termSize.cols);

    while(1){

        // Exit if the flag is set to false. Only done by frame destructor
        if(f_renderActive == false){
            return;
        }
        renderMutex.lock(); 
        printFrameWorker();
        renderMutex.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds((uint16_t)1000/frameRate)); // Pause for a short time
    }
}

void frame::clearFrameWorker(void){

    renderMutex.lock();

    
    // Clear the terminal and create the border
    if(f_setBorder == true){
        buildBorder();

    }else{
        
        // Update terminal size
        s_size termSize = getTerminalSize();  

        // Create a string for an empty row of the frame
        std::string blankRow(termSize.cols, ' ');
        blankRow[blankRow.size() - 1] = '\n';
        // Fill the file with the current empty frame
        for(uint16_t n = 1; n < termSize.rows; n++){
            writeToFile(blankRow, n*termSize.cols);
        }
    }

    renderMutex.unlock();
}

void frame::clearFrame(void){

    // Check if the render mutex is acquired
    if(f_renderLockAcquired == true){
        std::cerr << "Frame render lock acquired elsewhere. Did you call releaseFrameRenderer()?" << std::endl;
        return;
    }

    clearFrameWorker();
}

void frame::buildBorder(void){

    // Update terminal size
    s_size termSize = getTerminalSize();  

    //Create a string for an empty row of the frame
    std::string blankRow(termSize.cols, ' ');
    // Set the border character at the start and end
    blankRow[1] = borderChar;
    blankRow[blankRow.size() - 2] = borderChar;
    blankRow[blankRow.size() - 1] = '\n';
    // Fill the second row from the beginning and end
    // with the current empty frame
    for(uint16_t n = 1; n < termSize.rows - 1; n++){
        // Update entire row
        writeToFile(blankRow, n * termSize.cols);
    }

    // Fill the blankRow with the border character
    blankRow.assign(blankRow.size(),borderChar);
    blankRow[0] = ' ';
    blankRow[blankRow.size() - 1] = '\n';

    // Fill the first and last row of the frame
    writeToFile(blankRow, termSize.cols*(termSize.rows - 1));
    writeToFile(blankRow, termSize.cols);
}

void frame::enableBorder(char border){
    
    borderChar = border;
    f_setBorder = true;
}

void frame::enableBorder(){

    f_setBorder = true;
}

void frame::disableBorder(){

    f_setBorder = false;
}

s_size frame::getFrameSize(void){

    s_size size;

    if(f_setBorder == true){
        size = getBorderedFrameSize();

    }else{
        size = getTerminalSize();
    }

    return size;
}


s_size frame::getBorderedFrameSize(void){

    // Update terminal size
    s_size termSize = getTerminalSize();  
    s_size borderedFrameSize;

    // Set the frame size variable
    borderedFrameSize.cols = termSize.cols 
                                - FRAME_BUFFER 
                                - 2*FRAME_BORDER_WIDTH - 1;    // 1 space, 2 border, 1 '\n'

    borderedFrameSize.rows = termSize.rows - 
                                2*FRAME_BORDER_WIDTH + FRAME_BUFFER;    // 1 space, 2 border

    return borderedFrameSize;
}

void frame::lockFrameRenderer(){

    renderMutex.lock();
    f_renderLockAcquired = true;
}

void frame::releaseFrameRenderer(){

    renderMutex.unlock();
    f_renderLockAcquired = false;
}

void frame::updateFPS(fps fps){

    if((fps > 0)&&(fps < 500)){
        frameRate = fps;
    }else{
        std::cerr << "Invalid FPS provided" << std::endl;
    }
}

void frame::updateFrameElement(char c, int16_t row, int16_t col){  // Update the frame in temp file

    // Check if the render mutex is acquired
    if(f_renderLockAcquired == false){
        std::cerr << "Frame render lock missing. Did you call LockFrameRenderer()?" << std::endl;
        return;
    }

    s_size frameSize = getFrameSize();

    if(f_setBorder == true){
        // Check if row is valid
        if((row < 1) || (row > frameSize.rows)){
            std::cerr << "Requested row violates frame border" << std::endl;
            return;
        }   

        // Check if column is valid
        if((col < -1) || (col > frameSize.cols)){
            std::cerr << "Requested column violates frame border" << std::endl;
            return;
        }
       
        // Update the character in the temp frame file at the provided position
        writeToFile(c, row*(FRAME_OFFSET + frameSize.cols + FRAME_OFFSET) + FRAME_OFFSET + col);
    
    }else{ 

        // Check if row is valid
        if((row < 1) || (row > frameSize.rows)){
            std::cerr << "Requested row violates frame border" << std::endl;
            return;
        }   

        // Check if column is valid
        if((col < 1) || (col > frameSize.cols - 1)){
            std::cerr << "Requested column violates frame border" << std::endl;
            return;
        }

        // Update the character in the temp frame file at the provided position
        writeToFile(c, row*frameSize.cols + col);
    }

}


void frame::updateFrameRow(const std::string& c, int16_t row){                  // Update the frame in temp file

    // Check if the render mutex is acquired
    if(f_renderLockAcquired == false){
        std::cerr << "Frame render lock missing. Did you call LockFrameRenderer()?" << std::endl;
        return;
    }

    s_size frameSize = getFrameSize();

    if(f_setBorder == true){
        
        // Check if row is valid
        if((row < 1) || (row > frameSize.rows)){
            std::cerr << "Requested row exceeds terminal size" << std::endl;
            return;
        }

        // Check size of the string
        if((uint16_t)c.length() != frameSize.cols){
            std::cerr << "String to be printed is not the same size as row" << std::endl;
            return;
        }

        // Update entire row
        writeToFile(c, row * frameSize.cols);

    }else{

        // Check if row is valid
        if((row < 1) || (row > frameSize.rows)){
            std::cerr << "Requested row exceeds terminal size" << std::endl;
            return;
        }

        // Check size of the string
        if((uint16_t)c.length() != frameSize.cols){
            std::cerr << "String to be printed is not the same size as row" << std::endl;
            return;
        }

        // Update entire row
        writeToFile(c, row * frameSize.cols);
    }

}

void frame::writeToFile(const std::string& c, uint16_t pos){
        
    frameFileOut.seekp(pos, std::ios_base::beg);
    frameFileOut << c << std::flush;
}

void frame::writeToFile(const char c, uint16_t pos){

    frameFileOut.seekp(pos, std::ios_base::beg);
    frameFileOut << c << std::flush;
}

void frame::printFrameWorker(void){
    
    // Clear terminal
    std::cout << "\x1b[2J\x1b[H" << std::flush; 
    std::cout << "\033[1m"; 

    // Print temp file to terminal
    frameFileIn.seekg(0, std::ios::beg); // Move the pointer to the beginning
    std::cout << frameFileIn.rdbuf(); // Copy the file's buffer directly to cout

    // Print debug info if available
    if(debugInfoMap.size() > 0){
        for(const auto& info: debugInfoMap){
            std::cout << "\r" << info.first << ": " << info.second << std::endl;
        }
    }
}

void frame::printFrame(void){  // Print frame to terminal

    if(f_renderActive == true){
        std::cout << "Frame initialised to print via internal thread" << std::endl;
        return;
    }
    
    printFrameWorker();
}

s_size frame::getTerminalSize(void){

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    s_size termSize;

    termSize.cols = w.ws_col + 1;
    termSize.rows = w.ws_row - static_cast<uint16_t>(debugInfoMap.size());

    // Add space for debug data
    //termSize.rows -= static_cast<uint16_t>(debugInfoMap.size());

    return termSize;
}

bool frame::isWithinFrame(s_pos pos){
  
    s_size frameSize = getFrameSize();

    // Check if row is valid
    if((pos.y < 1) || (pos.y > frameSize.rows)){
        return false;
    }   

    // Check if column is valid
    if((pos.x < 0) || (pos.x > frameSize.cols)){
        return false;
    }
    
    return true;
}

void frame::addDebugInfo(const std::string& literal, int16_t value){

    // If already mapped, update the value
    if(debugInfoMap.count(literal) > 0){
        debugInfoMap[literal] = value;
        return;
    }

    // Append debug literal and value to the hash map
    debugInfoMap.emplace(literal, value);

}

void frame::addDebugInfo(const std::string& literal, char value){

    // If already mapped, update the value
    if(debugInfoMap.count(literal) > 0){
        debugInfoMap[literal] = value;
        return;
    }

    // Append debug literal and value to the hash map
    debugInfoMap.emplace(literal, value);

}

