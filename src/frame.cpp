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

    // Create a string for an empty row of the frame
    std::string blankRow(termSize.cols - 2, ' ');
    blankRow.append("\n");

    // Fill the file with the current empty frame
    for(uint16_t n = 0; n < termSize.rows; n++){
        updateFrameRow(blankRow, n);
    }
    
    // Print to terminal
    //printFrame();
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

void frame::resizeFrame(void){                 // Check size and rebuild frame in temp file

    // Get terminal size
    // Set size variables
    termSize = getTerminalSize();

}

void frame::updateFrameElement(char c, uint16_t row, uint16_t col){  // Update the frame in temp file

    // Check if row and col are valid
    if(row > termSize.rows || col > termSize.cols){
        std::cerr << "Requested row or column exceeds terminal size" << std::endl;
        return;
    }

    // Update the character in the temp frame file at the provided position
    frameFileOut.seekp(row*termSize.cols + col, std::ios_base::beg);
    frameFileOut << c << std::flush;
}

void frame::updateFrameRow(const std::string c, uint16_t row){                  // Update the frame in temp file

    // Check if row is valid
    if(row > termSize.rows){
        std::cerr << "Requested row exceeds terminal size" << std::endl;
        return;
    }

    // Check size of the string
    if(c.length() > termSize.cols){
        std::cerr << "String to be printed exceeds dimensions of row" << std::endl;
        DEBUG_PRINT("Size of string to print: %ld\n", c.length());
        return;
    }

    // Update entire row
    frameFileOut.seekp(row*termSize.cols, std::ios_base::beg);
    frameFileOut << c << std::flush;
}

void frame::printFrame(void){  // Print frame to terminal

    // Clear terminal
    std::cout << "\x1b[2J\x1b[H" << std::flush; 

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

    DEBUG_PRINT ("lines %d\n", w.ws_row);
    DEBUG_PRINT ("columns %d\n", w.ws_col);

    return termSize;
}