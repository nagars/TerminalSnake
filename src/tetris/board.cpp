#include <random>           // For random number generation facilities
#include <iostream>
#include "board.h"

board::board(){

    boardMatrix.resize(tetrisBoardSize.rows);
    for(auto& row: boardMatrix){
        row.resize(tetrisBoardSize.cols, ' ');
    }

    blockStartPos.x = tetrisBoardSize.cols/2 - 1;
    blockStartPos.y = -4;

}

board::board(s_size size){

    blockStartPos = {.x = size.cols, .y = -4};
    blockStartPos.x /= 2 - 1;

    boardMatrix.resize(size.rows);
    for(auto row: boardMatrix){
        row.resize(size.cols, ' ');
    }
}

std::string board::getRow(uint8_t n){

    if(n >= boardMatrix.size()){
        return " ";
    }

    return boardMatrix[n];
}

std::vector<std::string> board::getMatrix(){

    return boardMatrix;
}

void board::lockShape(){

    for(auto &row: boardMatrix){
        for(auto &elem: row){
            if(elem == 'A')
                elem = 'P';
        }
    }

    shape.reset();  // delete shape object

}

void board::createNewShape(){

    // Another shape is active
    if(shape != NULL){
        std::cerr << "An active shape is in play!" << std::endl;
        return;
    }

    shape = std::unique_ptr<block>(new block(shapes[randomNumGen()]));
    shape->setPosition(blockStartPos);
}

uint8_t board::randomNumGen(){

    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator

    std::uniform_int_distribution<> distrib0(0, 7); // Define the distribution
    return (uint8_t)distrib0(gen); // Generate the random number

}

void board::shiftLeftShape(){

}
void board::shiftRightShape(){

}
void board::shiftDownShape(){

    // Get the position of the centre of the shape 
    // matrix on the board
    s_pos pos = shape->getPosition();
    blockMatrix shapeMatrix = shape->getMatrix();

    // Clear the current active shape in the board grid
    for(auto& row : boardMatrix){
        for (auto& elem : row){
            if(elem == 'A')
                elem = ' ';
        }
    }

    pos.y += 1;

    // Get coordinates of corners of the shape matrix
    // in relation to the centre position of the shape
    // on the board
    std::pair<s_pos, s_pos> dimension = shape->getDimension();
    s_pos topLeft = dimension.first;
    s_pos bottomRight = dimension.second;

    // Check if shape is outside of grid
    if(bottomRight.y >= 0){
        
        for(uint16_t row = 0; row < shapeMatrix.size(); row++){
            
            // Check if the row being checked is within the board
            int16_t boardRow = topLeft.y + row;
            if(boardRow >= 0 && boardRow < boardMatrix.size()){

                for(uint16_t col = 0; col < shapeMatrix[0].size(); col++){
                    int16_t boardCol = topLeft.x + col;

                    if(boardCol >= 0 && boardCol < boardMatrix[0].size()){
                        if(shapeMatrix[row][col] == '*'){
                            // Set element as active in board
                            boardMatrix[boardRow][boardCol] = 'A';
                        }
                    }
                }
            }
        }
    }

    // Update position
    shape->setPosition(pos);
}

void board::rotateRightShape(){

}
void board::rotateLeftShape(){

}