// Prints the game frame to terminal

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstdio> 
#include "frame.h"

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

    if(f_setBorder == true){

        // Create a string for an empty row of the frame
        std::string blankRow(termSize.cols, ' ');
        // Set the border character at the start and end
        blankRow[1] = borderChar;
        blankRow[blankRow.size() - 2] = borderChar;
        blankRow[blankRow.size() - 1] = '\n';
        // Fill the second row from the beginning and end
        // with the current empty frame
        for(uint16_t n = 2; n < termSize.rows - 1; n++){
            updateFrameRow(blankRow, n);
        }

        // Fill the blankRow with the border character
        blankRow.assign(blankRow.size(),borderChar);
        blankRow[blankRow.size() - 1] = '\n';

        // Fill the first and last row of the frame
        blankRow[0] = ' ';
        updateFrameRow(blankRow, termSize.rows - 1);
        updateFrameRow(blankRow, 1);


    }else{
        // Create a string for an empty row of the frame
        std::string blankRow(termSize.cols, ' ');
        blankRow[blankRow.size() - 1] = '\n';
        // Fill the file with the current empty frame
        for(uint16_t n = 0; n < termSize.rows; n++){
            updateFrameRow(blankRow, n);
        }
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

    return termSize;
}


void frame::updateFrameElement(char c, uint16_t row, uint16_t col){  // Update the frame in temp file

    // Update terminal size
    termSize = getTerminalSize();  

    if(f_setBorder == true){
        // Check if row and col are valid
        if((row < 1) || (row > termSize.rows) || (col > termSize.cols) || (col < 1)){
            std::cerr << "Requested row or column exceeds terminal size" << std::endl;
            return;
        }

    }else{
        // Check if row and col are valid
        if((row > termSize.rows) || (col > termSize.cols)){
            std::cerr << "Requested row or column exceeds terminal size" << std::endl;
            return;
        }

    }

    // Check if this is working????????????????????????????????////
    // Update the character in the temp frame file at the provided position
    frameFileOut.seekp(row*termSize.cols + col, std::ios_base::beg);
    frameFileOut << c << std::flush;
}

void frame::updateFrameRow(const std::string& c, uint16_t row){                  // Update the frame in temp file

    // Update terminal size
    termSize = getTerminalSize();  

    if(f_setBorder == true){
        // Check if row is valid
        if((row < 0) || (row > termSize.rows - 1)){
            std::cerr << "Requested row exceeds terminal size or violates border" << std::endl;
            return;
        }   

        // Check size of the string
        if((uint16_t)c.length() != termSize.cols){
            std::cerr << "String to be printed is not the same size as row\n(Did you account for the border?)" << std::endl;
            DEBUG_PRINT("Size of string to print: %ld\n", c.length());
            return;
        }

        // Update entire row
        frameFileOut.seekp(row*termSize.cols, std::ios_base::beg);
        frameFileOut << c << std::flush;

    }else{
        // Check if row is valid
        if((row < 0) || (row > termSize.rows)){
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
        frameFileOut.seekp(row*termSize.cols, std::ios_base::beg);
        frameFileOut << c << std::flush;
    }

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
    
    s_size termSize;
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    termSize.cols = w.ws_col;
    termSize.rows = w.ws_row;

    return termSize;
}