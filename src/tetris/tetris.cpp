#include "tetris.h"
#include <thread>
#include <iostream>
#include <unistd.h> 
#include <algorithm>
#include <termios.h>        // For termios functions
#include <random>           // For random number generation facilities

#define PAUSE_COMMAND   32
#define MOVE_RIGHT_COMMAND  'd'
#define MOVE_LEFT_COMMAND   'a'
#define MOVE_DOWN_COMMAND   's'
#define ROTATE_RIGHT_COMMAND    '.'
#define ROTATE_LEFT_COMMAND     ','

s_pos originOffsetTettris;
s_pos originOffsetInfoLeft;

tetris::tetris() : frame(){

    // Change terminal settings to a non-blocking read
    struct termios attr;
    tcgetattr(STDIN_FILENO, &attr);     // Get current terminal attributes
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
    //originOffsetInfoLeft.x = (frameSize.cols - tetrisSize.cols - infoLeftSize.cols) / 2;
    //originOffsetTettris.x = originOffsetInfoLeft.x + infoLeftSize.cols;
    originOffsetTettris.x = frameSize.cols / 2 - tetrisSize.cols / 2;
    originOffsetInfoLeft.x = originOffsetTettris.x - infoLeftSize.cols;

    // Build default screen
    clearFrame();

    // Build the border
    buildGameBoard();
    buildInfoBoard();

    // Print Game
    printGame();

};

void tetris::run(){

    char cmd = '0';   // Stores command from user
    bool f_shapeActive = false;

    // // Start above the tetris frame in the middle column
    s_pos blockStartPos = {.x = originOffsetTettris.x + (int16_t)(tetrisSize.cols / 2), .y = -5};

    while(1){

        // If no object is active, select one
        if(f_shapeActive == false){
            shape = std::unique_ptr<block>(new block [randomNumGen()]);
            shape->setPosition(blockStartPos);
            f_shapeActive = true;
            continue;
        }

        // read command from terminal
        read(STDIN_FILENO, &cmd, 1);

        if(cmd == PAUSE_COMMAND){
            pause();
        }else{
            // If shape is active
            if(f_shapeActive == true){
                switch(cmd){
                    case MOVE_DOWN_COMMAND:
                    break;
                    case MOVE_LEFT_COMMAND:
                    break;
                    case MOVE_RIGHT_COMMAND:
                    break;
                    case ROTATE_LEFT_COMMAND:
                    break;
                    case ROTATE_RIGHT_COMMAND:
                    break;
                    default:
                    break;
                }
            }
        }cmd = 0;        // reset the command

       
        // drop further
        //moveShape();

        // check collision?

        // reset active object

        // Select a shape to load
        std::this_thread::sleep_for(std::chrono::milliseconds(200)); // Pause for a short time

    
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
 
    // Prepare a string to print to terminal
    std::string line(tetrisSize.cols,' ');
    line.assign(line.size(),' ');

    // Fill board
    line[0] = '<';
    line[1] = '!';
    line[line.size() - 2] = '!';
    line[line.size() - 1] = '>';
    
    // Each alternate element has a .
    for(uint16_t n = 2; n < line.size() - 2; n++){
        if(n % 2)
            line[n] = '.';
    }

    for(std::string &row: gameBoard){
        row = line;
    }

    // Fill second last line
    line.assign(line.size(),'=');

    line[0] = '<';
    line[1] = '!';
    line[line.size() - 2] = '!';
    line[line.size() - 1] = '>';

    //std::copy(line.begin(), line.end(), gameBoard[gameBoard.size() - 2].begin());
    gameBoard[gameBoard.size() - 2] = line;

    // Fill last line
    line.assign(line.size(),' ');
    for(uint16_t n = 2; n < line.size() - 2; n++){
        if(n % 2)
            line[n] = '/';
        else
            line[n] = '\\';
    }

    gameBoard[gameBoard.size() - 1] = line;
}

void tetris::printGame(){

    s_size frameSize = getFrameSize();
    std::string row(frameSize.cols,' ');

    updateFrameRow(row, 0);
    for(uint16_t n = 1; n < gameBoard.size(); n++){
        std::string boardRow = gameBoard[n];
        row.replace(originOffsetTettris.x, boardRow.size(), boardRow);
        //std::copy(boardRow.begin(), boardRow.end(), row.at(originOffsetTettris.x));
        updateFrameRow(row, n);
    }

    printFrame();
}

uint8_t tetris::randomNumGen(){

    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator

    std::uniform_int_distribution<> distrib0(0, 7); // Define the distribution
    return (uint8_t)distrib0(gen); // Generate the random number

}