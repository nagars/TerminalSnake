// Prints the game frame to terminal

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <algorithm> // for copy
#include <iterator> // for ostream_iterator
#include <cstdio> 
#include <thread>
#include "frame.h"

frame::frame(void){

    // Create the matrix used for printing
    resizeDisplayMatrix();
    
    // std::cout << '\r' << std::endl;

    // Set the frame render lock flag to true
    //f_renderLockAcquired = true;

    clearFrameWorker();
}

frame::frame(char border):borderChar(border){

    // Create the matrix used for printing
    resizeDisplayMatrix();

    // Set the border flag to true
    f_setBorder = true;
    
    // Set the frame render lock flag to true
    //f_renderLockAcquired = true;

    clearFrameWorker();
}

frame::frame(fps fps){

    // Create the matrix used for printing
    resizeDisplayMatrix();
    
    clearFrameWorker();

    // Set class fps variable
    updateFPS(fps);

    // Initialise the mutex. Do not lock
    std::unique_lock<std::mutex> lock(renderMutex, std::defer_lock);

    // Start the frame render worker thread
    frameRenderThread = std::thread(&frame::frameRenderWorker, this);
    
}

frame::frame(fps fps, char border):borderChar(border){

    // Create the matrix used for printing
    resizeDisplayMatrix();

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

    // Close thread
    if(f_renderActive == true){
        if(frameRenderThread.joinable() == true){
            f_renderActive = false;     // Trigger the thread to return

            frameRenderThread.join();
        }
    }

}

s_size frame::resizeDisplayMatrix(void){

    static s_size prevTermSize; 
    s_size termSize = getTerminalSize();

    if((termSize.rows != prevTermSize.rows) || (termSize.cols != prevTermSize.cols)){
    //std::cout << "Cols , Rows : " << termSize.cols << "," << termSize.rows << std::endl;

        displayMatrix.resize(termSize.rows);
        for(auto &row : displayMatrix){
            row.resize(termSize.cols);
            row.assign(termSize.cols,' ');
        }
    }

    prevTermSize = termSize;

    return termSize;
}

void frame::frameRenderWorker(void){
    // Set the renderer active flag
    f_renderActive = true;

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

    // Resize display matrix in case the 
    // terminal size changes
    resizeDisplayMatrix();

    // Clear all rows
    for(auto &row: displayMatrix){
        row.assign(row.size(),' ');
    }

    // If a border is required
    if(f_setBorder == true){

        for(auto &row: displayMatrix){
            row[0] = borderChar;
            row[row.size() - 1] = borderChar;
        }
        
        displayMatrix[0].assign(displayMatrix[0].size(), borderChar);
        displayMatrix[displayMatrix.size() - 1].assign(displayMatrix[0].size(), borderChar);
    }

    // s_size termSize = getTerminalSize();
    // for(auto row: displayMatrix){
    //     row.assign(termSize.cols,' ');
    // }

    // Clear the terminal and create the border
    // if(f_setBorder == true){
    //     for(auto row: displayMatrix){
    //         row[0] = borderChar;
    //         row[termSize.cols - 1] = borderChar;
    //     }
    //     displayMatrix[0].assign(termSize.cols, borderChar);
    //     displayMatrix[termSize.cols - 1].assign(termSize.cols, borderChar);
    // }
}

void frame::clearFrame(void){

    // Check if the render mutex is acquired
    if(f_renderActive == true){
        if(f_renderLockAcquired == true){
            std::cerr << "Frame render lock acquired elsewhere. Did you call releaseFrameRenderer()?" << std::endl;
            return;
        }

        renderMutex.lock();
        clearFrameWorker();
        renderMutex.unlock();
        
    }else{
        clearFrameWorker();
    }
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
    termSize.cols -= 2;
    termSize.rows -= 2;
    return termSize;
}

void frame::lockFrameRenderer(){

    if(f_renderActive == false){
        return;
    }

    renderMutex.lock();
    f_renderLockAcquired = true;
}

void frame::releaseFrameRenderer(){

    if(f_renderActive == false){
        return;
    }

    renderMutex.unlock();
    f_renderLockAcquired = false;
}

void frame::updateFPS(fps fps){

    if(f_renderActive == false){
        return;
    }

    if((fps > 0)&&(fps < 500)){
        frameRate = fps;
    }else{
        std::cerr << "Invalid FPS provided" << std::endl;
    }
}

void frame::updateFrameElement(char c, int16_t row, int16_t col){  // Update the frame in temp file
 
    // Check if the render mutex is acquired
    if(f_renderActive == true && f_renderLockAcquired == false){
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
        if((col < 1) || (col > frameSize.cols)){
            std::cerr << "Requested column violates frame border" << std::endl;
            return;
        }
       
    }else{ 

        // Check if row is valid
        if((row < 0) || (row > frameSize.rows - 1)){
            std::cerr << "Requested row violates frame border" << std::endl;
            return;
        }   

        // Check if column is valid
        if((col < 0) || (col > frameSize.cols - 1)){
            std::cerr << "Requested column violates frame border" << std::endl;
            return;
        }
    }


    // Update the character in the temp frame file at the provided position
    // try{
        displayMatrix[row].at(col) = c;
    
    // }catch(const std::out_of_range& e){
    //     std::cerr << "Exception caught in file: " << __FILE__ 
    //              << ", line: " << __LINE__ 
    //              << ". Error: " << e.what() << std::endl;
    // }
}


void frame::updateFrameRow(const std::string& c, int16_t row){                  // Update the frame in temp file

    // Check if the render mutex is acquired
    if(f_renderActive == true && f_renderLockAcquired == false){
        std::cerr << "Frame render lock missing. Did you call LockFrameRenderer()?" << std::endl;
        return;
    }

    s_size frameSize = getFrameSize();

    if(f_setBorder == true){
        
        // Check if row is valid
        if((row < 1) || (row > frameSize.rows - 1)){
            std::cerr << "Requested row exceeds terminal size" << std::endl;
            return;
        }

    }else{

        // Check if row is valid
        if((row < 0) || (row > frameSize.rows)){
            std::cerr << "Requested row exceeds terminal size" << std::endl;
            return;
        }

    }

    // Check size of the string
    if((uint16_t)c.length() != frameSize.cols){
        std::cerr << "String to be printed is not the same size as row" << std::endl;
        return;
    }

    // Update entire row
    for(uint16_t n = 0; n < c.length(); n++){
        displayMatrix[row].at(n) = c[n];
    }
}

void frame::printFrameWorker(void){
    
    // Clear terminal
    std::cout << "\x1b[2J\x1b[H" << std::flush; 
    std::cout << "\033[1m"; 

    // Print matrix to terminal
    for(auto row: displayMatrix){
        std::copy(row.begin(), row.end(), std::ostream_iterator<char>(std::cout));
        std::cout << std::endl;
    }

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

    termSize.cols = w.ws_col;
    // Subtract the row required for debug information
    termSize.rows = w.ws_row - 1 - static_cast<uint16_t>(debugInfoMap.size());

    return termSize;
}

bool frame::isWithinFrame(s_pos pos){
  
    s_size frameSize = getFrameSize();

    if(f_setBorder == true){
        // Check if row is valid
        if((pos.y < 0) || (pos.y > frameSize.rows + 1)){
            return false;
        }   

        // Check if column is valid
        if((pos.x < 0) || (pos.x > frameSize.cols + 1)){
            return false;
        }

    }else{
        // Check if row is valid
        if((pos.y < -1) || (pos.y > frameSize.rows)){
            return false;
        }   

        // Check if column is valid
        if((pos.x < 0) || (pos.x > frameSize.cols)){
            return false;
        }
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

