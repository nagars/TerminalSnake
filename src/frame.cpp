// Prints the game frame to terminal

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstdio> 
#include "frame.h"

#define FRAME_BUFFER    1
#define FRAME_BORDER_WIDTH  1

#define FILE_OFFSET FRAME_BUFFER + FRAME_BORDER_WIDTH

frame::frame(void){

    // Get the terminal size and build the frame limits
    // set termSize variable
    termSize = getTerminalSize();

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
    clearFrame();
}

frame::frame(char border){

    // Get the terminal size and build the frame limits
    // set termSize variable
    termSize = getTerminalSize();

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
    borderChar = border;

    // Set the frame size variable
    getBorderedFrameSize();
    
    clearFrame();
}

frame::~frame(void){

    // Close pointer to temp file
    frameFileOut.close();
    frameFileIn.close();

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

void frame::clearFrame(void){

    // Update terminal size
    termSize = getTerminalSize();  

    // Clear the terminal and create the border
    if(f_setBorder == true){
        buildBorder();

    }else{
        // Create a string for an empty row of the frame
        std::string blankRow(termSize.cols, ' ');
        blankRow[blankRow.size() - 1] = '\n';
        // Fill the file with the current empty frame
        for(uint16_t n = 1; n < termSize.rows; n++){
            updateFrameRow(blankRow, n);
        }
    }

}

void frame::buildBorder(void){

    // Update terminal size
    getTerminalSize();  

    //Create a string for an empty row of the frame
    std::string blankRow(termSize.cols, ' ');
    // Set the border character at the start and end
    blankRow[1] = borderChar;
    blankRow[blankRow.size() - 2] = borderChar;
    blankRow[blankRow.size() - 1] = '\n';
    // Fill the second row from the beginning and end
    // with the current empty frame
    for(uint16_t n = 2; n < termSize.rows - 1; n++){
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

    return termSize;
}

s_size frame::getBorderedFrameSize(void){

    // Update terminal size
    getTerminalSize();  

    // Set the frame size variable
    borderedFrameSize.cols = termSize.cols - FRAME_BUFFER - 2*FRAME_BORDER_WIDTH - 1;    // 1 space, 2 border, 1 '\n'
    borderedFrameSize.rows = termSize.rows - FRAME_BUFFER - 2*FRAME_BORDER_WIDTH;    // 1 space, 2 border

    return borderedFrameSize;
}


void frame::updateFrameElement(char c, uint16_t row, uint16_t col){  // Update the frame in temp file

    if(f_setBorder == true){

        // Update size of the border
        getBorderedFrameSize();

        // Check if row is valid
        if((row < 1) || (row > borderedFrameSize.rows)){
            std::cerr << "Requested row violates frame border" << std::endl;
            return;
        }   

        // Check if column is valid
        if((col < 1) || (col > borderedFrameSize.cols)){
            std::cerr << "Requested column violates frame border" << std::endl;
            return;
        }

        // Update the character in the temp frame file at the provided position
        if(row == 1){
            writeToFile(c, FILE_OFFSET + col);

        }else if(row > 1){
            writeToFile(c, row*(FILE_OFFSET + borderedFrameSize.cols + FILE_OFFSET) + col);
        }

    }else{

        // Update terminal size
        getTerminalSize();  

        // Check if row is valid
        if((row < 1) || (row > termSize.rows)){
            std::cerr << "Requested row violates frame border" << std::endl;
            return;
        }   

        // Check if column is valid
        if((col < 1) || (col > termSize.cols - 1)){
            std::cerr << "Requested column violates frame border" << std::endl;
            return;
        }

        // Update the character in the temp frame file at the provided position
        writeToFile(c, row*termSize.cols + col);
    }


}


void frame::updateFrameRow(const std::string& c, uint16_t row){                  // Update the frame in temp file


    if(f_setBorder == true){

        // Update size of the border
        getBorderedFrameSize();
        
        // Check if row is valid
        if((row < 1) || (row > borderedFrameSize.rows)){
            std::cerr << "Requested row exceeds terminal size" << std::endl;
            return;
        }

        // Check size of the string
        if((uint16_t)c.length() != borderedFrameSize.cols){
            std::cerr << "String to be printed is not the same size as row" << std::endl;
            DEBUG_PRINT("Size of string to print: %ld\n", c.length());
            return;
        }

        // Update entire row
        writeToFile(c, row * borderedFrameSize.cols);

    }else{

        // Update terminal size
        getTerminalSize();  

        // Check if row is valid
        if((row < 1) || (row > termSize.rows - 1)){
            std::cerr << "Requested row exceeds terminal size" << std::endl;
            return;
        }

        // Check size of the string
        if((uint16_t)c.length() != termSize.cols){
            std::cerr << "String to be printed is not the same size as row" << std::endl;
            DEBUG_PRINT("Size of string to print: %ld\n", c.length());
            return;
        }

        // Update entire row
        writeToFile(c, row * termSize.cols);
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



void frame::printFrame(void){  // Print frame to terminal

    DEBUG_PRINT ("lines %d\n", termSize.rows);
    DEBUG_PRINT ("columns %d\n", termSize.cols);

    // Clear terminal
    std::cout << "\x1b[2J\x1b[H" << std::flush; 
    std::cout << "\033[1m"; 

    // Print temp file to terminal
    frameFileIn.seekg(0, std::ios::beg); // Move the pointer to the beginning
    std::cout << frameFileIn.rdbuf(); // Copy the file's buffer directly to cout

}

s_size frame::getTerminalSize(void){
    
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    termSize.cols = w.ws_col;
    termSize.rows = w.ws_row;

    return termSize;
}