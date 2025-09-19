#include "tetris.h"
#include <thread>
#include <iostream>
#include <unistd.h> 
#include <algorithm>
#include <termios.h>        // For termios functions


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
    originOffsetTettris.x = frameSize.cols / 2 - tetrisSize.cols / 2;
    originOffsetInfoLeft.x = originOffsetTettris.x - infoLeftSize.cols;

    // Build default screen
    clearFrame();

    // Build the border
    buildGameBoard();
    buildInfoBoard();

    // Print Game
    //printGame();

};

void tetris::run(){

    char cmd = '0';   // Stores command from user
    bool f_shapeActive = false;

    // Start above the tetris frame in the middle column

    while(1){

        // If no object is active, select one at random
        if(f_shapeActive == false){
            // shape = std::unique_ptr<block>(new block(shapes[randomNumGen()]));
            // shape->setPosition(blockStartPos);
            gameBoard.createNewShape();
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
        gameBoard.shiftDownShape();

        //printGame();
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



void tetris::updateGameBoard(){

    // // get position of shape
    // blockMatrix matrix = shape->getMatrix();

    // // check how much of the shape should be printed 
    // // to the board 
    // std::pair<s_pos,s_pos> dimension = shape->getDimension();

    // // Check if the shape is completely within the board
    // // Left top corner and right bottom corners are within the board
    // if((dimension.first.y >= 0) && (dimension.first.x >= 0) && (dimension.second.x < tetrisBoardSize.cols)){
    //     // Print entire shape
    //     s_pos posCornerElement = dimension.first; // Grid position of first element of shape

    //     for(uint16_t row = 0; row < matrix.size(); row++){
    //         for(uint16_t col = 0; col < matrix[row].size(); col++){

    //             char shapeElement = matrix[row][col];
    //             // If element is part of shape, insert it into grid array
    //             if(shapeElement == '[' || shapeElement == ']'){
    //                 gameBoard[posCornerElement.y + row][posCornerElement.x + col] = shapeElement;
    //             }
    //         }
    //     }
    
    // }else{
    //     // Only print rows that have a position greater than 0

    // }

    // Update game board

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

    for(uint16_t m = 0; m < tetrisSize.rows - 2; m++){

        uint16_t offset = originOffsetInfoLeft.x - 1;
        // assign info box
        // line.replace()

        offset = originOffsetTettris.x - 1;
        // assign tetris board
        line[offset++] = '<';
        line[offset++] = '!';

        // Fill board with empty blocks
        for(uint16_t n = 0; n < tetrisSize.cols - 2; n++){
            if(n % 2)
                line[n + offset] = '.';
        }
        offset += tetrisSize.cols - 2;

        // Fill sections where shapes exist
        std::string boardM = gameBoard.getRow(m);
        for(uint16_t l = 0; l < boardM.size(); l++){
            if(boardM[l] == 'A' || boardM[l] == 'L'){
                line[2*l] = '[';
                line[2*l + 1] = ']';
            }
        }

        line[offset++] = '>';
        line[offset++] = '!';

        // Update matrix to print to terminal
        updateFrameRow(line, m);
    }

    // Fill second last line
    line.replace((size_t)originOffsetTettris.x + 1, (size_t)tetrisSize.cols - 2, (size_t)tetrisSize.cols - 2,'=');

    // line[0] = '<';
    // line[1] = '!';
    // line[line.size() - 2] = '!';
    // line[line.size() - 1] = '>';

    updateFrameRow(line, tetrisSize.rows - 2);

    // Fill last line
    line.assign(line.size(),' ');
    for(uint16_t n = originOffsetTettris.x + 1; n < originOffsetTettris.x + 1 + (size_t)tetrisSize.cols - 2; n++){
        if(n % 2)
            line[n] = '/';
        else
            line[n] = '\\';
    }

    updateFrameRow(line, tetrisSize.rows - 1);

    printFrame();
}

// void tetris::buildGameBoard(){
 
//     // Prepare a string to print to terminal
//     std::string line(tetrisSize.cols,' ');
//     line.assign(line.size(),' ');

//     // Fill board
//     line[0] = '<';
//     line[1] = '!';
//     line[line.size() - 2] = '!';
//     line[line.size() - 1] = '>';
    
//     // Each alternate element has a .
//     for(uint16_t n = 2; n < line.size() - 2; n++){
//         if(n % 2)
//             line[n] = '.';
//     }


//     // for(std::string &row: gameBoard){
//     //     row = line;
//     // }

//     // Fill second last line
//     line.assign(line.size(),'=');

//     line[0] = '<';
//     line[1] = '!';
//     line[line.size() - 2] = '!';
//     line[line.size() - 1] = '>';

//     //std::copy(line.begin(), line.end(), gameBoard[gameBoard.size() - 2].begin());
//     gameBoard[gameBoard.size() - 2] = line;

//     // Fill last line
//     line.assign(line.size(),' ');
//     for(uint16_t n = 2; n < line.size() - 2; n++){
//         if(n % 2)
//             line[n] = '/';
//         else
//             line[n] = '\\';
//     }

//     gameBoard[gameBoard.size() - 1] = line;
// }

// void tetris::printGame(){

//     s_size frameSize = getFrameSize();
//     std::string row(frameSize.cols,' ');

//     updateFrameRow(row, 0);
//     for(uint16_t n = 1; n < gameBoard.size(); n++){
//         row.replace(originOffsetTettris.x, gameBoard[n].size(), gameBoard[n]);
//         updateFrameRow(row, n);
//     }

//     printFrame();
// }

