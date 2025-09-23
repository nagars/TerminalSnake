#include "tetris.h"
#include <thread>
#include <iostream>
#include <unistd.h> 
#include <algorithm>
#include <termios.h>        // For termios functions

#define LOOPDELAY   100

#define PAUSE_COMMAND   32
#define MOVE_RIGHT_COMMAND  'd'
#define MOVE_LEFT_COMMAND   'a'
#define MOVE_DOWN_COMMAND   's'
#define ROTATE_RIGHT_COMMAND    '.'
#define ROTATE_LEFT_COMMAND     ','

s_pos originOffsetTettris;
s_pos originOffsetInfoLeft;

struct termios oldAttr;

tetris::tetris() : frame(){

    // Change terminal settings to a non-blocking read
    struct termios attr;
    tcgetattr(STDIN_FILENO, &attr);     // Get current terminal attributes
    oldAttr = attr;
    attr.c_lflag &= ~(ICANON | ECHO);   // Disable canonical mode and echoing
    attr.c_cc[VMIN] = 0;                // Read returns immediately (Polling / non-blocking)
    attr.c_cc[VTIME] = 0;               // Read has no timeout
    tcsetattr(STDIN_FILENO, TCSANOW, &attr); // Apply new terminal attributes

    // Check if the terminal size is large enough
    s_size frameSize = getFrameSize();
    if((frameSize.cols < tetrisSize.cols + infoLeftSize.cols)||
            (frameSize.rows < tetrisSize.rows)){
        std::cerr << "Error: Terminal size is too small!\n" << std::endl;
        exit(0);
    }

    // Define the start position offset
    originOffsetTettris.x = frameSize.cols / 2 - tetrisSize.cols / 2;
    originOffsetInfoLeft.x = originOffsetTettris.x - infoLeftSize.cols;

    // Build default screen
    clearFrame();

    // Build the border
    buildGameBoard();
    buildInfoBoard();

};

tetris::~tetris(){
    // reset terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldAttr); // Restore original terminal settings
}

void tetris::run(){

    char cmd = '0';   // Stores command from user
    bool f_shapeActive = false;

    // Start above the tetris frame in the middle column

    while(1){

        cmd = '0';

        // Select a shape to load
        std::this_thread::sleep_for(std::chrono::milliseconds(LOOPDELAY)); // Pause for a short time

        buildGameBoard();

        // Check if a line has been completed
        if(gameBoard.checkLineComplete() == true)
            continue;

        // Check is a new shape is required
        f_shapeActive = gameBoard.getShapeActiveStatus();

        // If no object is active, select one at random
        if(f_shapeActive == false){
            gameBoard.createNewShape();
            f_shapeActive = true;
            continue;
        }

        // read command from terminal
        read(STDIN_FILENO, &cmd, 1);

        if(cmd == PAUSE_COMMAND){
            pause();
            continue;
        }

        // If shape is active
        if(f_shapeActive == true){
            switch(cmd){
                case MOVE_DOWN_COMMAND:
                gameBoard.shiftDownShape();
                std::this_thread::sleep_for(std::chrono::milliseconds(20)); // Pause for a short time
                continue;
                break;
                case MOVE_LEFT_COMMAND:
                gameBoard.shiftLeftShape();
                break;
                case MOVE_RIGHT_COMMAND:
                gameBoard.shiftRightShape();
                break;
                case ROTATE_LEFT_COMMAND:
                gameBoard.rotateLeftShape();
                break;
                case ROTATE_RIGHT_COMMAND:
                gameBoard.rotateRightShape();
                break;
                default:
                gameBoard.shiftDownShape();
                break;
            }
        }

    }
}

void tetris::pause(){
    
    char cmd;
    while(cmd != PAUSE_COMMAND){
        read(STDIN_FILENO, &cmd, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Pause for a short time
    }

}
// void tetris::end(){


// }

// void tetris::clearLine(){


// }

// void tetris::clearScreen(){


// }

// void tetris::collisionBorder(){


// }

void tetris::buildInfoBoard(){

}


/**
 * <! . . . . .!>
 * <! . . . . .!>
 * <! . . . . .!>
 * <! . . . . .!>
 * <! . . . . .!>
 * <!==========!>
 *   \/\/\/\/\/
 */

void tetris::buildGameBoard(){


    s_size frame = getFrameSize();
    std::string line(frame.cols, ' ');
    uint16_t offset;

    for(uint16_t m = 0; m < tetrisSize.rows - 2; m++){

        offset = originOffsetInfoLeft.x - 1;
        // assign info box
        // line.replace()

        offset = originOffsetTettris.x - 1;
        // Fill left boundary
        line[offset++] = '<';
        line[offset++] = '!';

        // Fill board with empty blocks
        for(uint16_t n = 0; n < tetrisSize.cols - 4; n++){
            if(n % 2)
                line[n + offset] = '.';
            else
                line[n + offset] = ' ';
        }

        // Fill sections where shapes exist
        std::string boardM = gameBoard.getRow(m);
        for(uint16_t l = 0; l < boardM.size(); l++){
            if(boardM[l] == 'A' || boardM[l] == 'L'){
                line[offset + 2*l] = '[';
                line[offset + 2*l + 1] = ']';
            }
        }
        // Fill the right boundary
        offset += tetrisSize.cols - 4;
        line[offset++] = '!';
        line[offset++] = '>';

        // Update matrix to print to terminal
        updateFrameRow(line, m);
    }

    offset = originOffsetTettris.x + 1;
    // Fill second last line
    line.replace((size_t)offset, (size_t)tetrisSize.cols - 4, (size_t)tetrisSize.cols - 4,'=');
    updateFrameRow(line, tetrisSize.rows - 2);

    // Fill last line
    line.assign(line.size(),' ');
    for(uint16_t n = offset; n < originOffsetTettris.x + 1 + (size_t)tetrisSize.cols - 4; n++){
        if(n % 2)
            line[n] = '/';
        else
            line[n] = '\\';
    }
    updateFrameRow(line, tetrisSize.rows - 1);

    printFrame();
}


