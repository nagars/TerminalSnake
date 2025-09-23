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
                elem = 'L';
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

    std::uniform_int_distribution<> distrib0(0, 6); // Define the distribution
    return (uint8_t)distrib0(gen); // Generate the random number

}

bool board::getShapeActiveStatus(){
    
    if(shape == NULL){
        return false;
    }
    return true;
}

void board::shiftLeftShape(){

    if(shape == NULL){
        std::cerr << "No active shape is in play!" << std::endl;
        return;
    }

    // Get the position of the centre of the shape 
    // matrix on the board
    s_pos newPos = shape->getPosition();
    newPos.x -= 1;
    s_collisionStatus status = checkCollisionMove(newPos);
    switch(status){
        case NO_COLLISION:
                moveShape(newPos);
                break;
        case SIDE_COLLISION:
                break;
        case BOTTOM_COLLISION:
        case SHAPE_COLLISION:
            lockShape();
            break;
        default:
        break;
    }
}

void board::shiftRightShape(){
    
    if(shape == NULL){
        std::cerr << "No active shape is in play!" << std::endl;
        return;
    }

    // Get the position of the centre of the shape 
    // matrix on the board
    s_pos newPos = shape->getPosition();
    newPos.x += 1;
    s_collisionStatus status = checkCollisionMove(newPos);
    switch(status){
        case NO_COLLISION:
                moveShape(newPos);
                break;
        case SIDE_COLLISION:
                break;
        case BOTTOM_COLLISION:
        case SHAPE_COLLISION:
            lockShape();
            break;
        default:
        break;
    }
}

void board::shiftDownShape(){

    if(shape == NULL){
        std::cerr << "No active shape is in play!" << std::endl;
        return;
    }

    // Get the position of the centre of the shape 
    // matrix on the board
    s_pos newPos = shape->getPosition();
    newPos.y += 1;
    s_collisionStatus status = checkCollisionMove(newPos);
    switch(status){
        case NO_COLLISION:
                moveShape(newPos);
                break;
        case SIDE_COLLISION:
                break;
        case BOTTOM_COLLISION:
        case SHAPE_COLLISION:
            lockShape();
            break;
        default:
        break;
    }
}


s_collisionStatus board::checkCollision(block& tempShape){

    blockMatrix shapeMatrix = tempShape.getMatrix();

    std::pair<s_pos, s_pos> dimension = tempShape.getDimension();
    s_pos topLeft = dimension.first;
    s_pos bottomRight = dimension.second;

    for(uint16_t row = 0; row < shapeMatrix.size(); row++){
        int16_t boardRow = topLeft.y + row;
        for(uint16_t col = 0; col < shapeMatrix[0].size(); col++){
            int16_t boardCol = topLeft.x + col;
            if(shapeMatrix[row][col] == '*'){
                if((boardCol < 0) || ((boardCol >= boardMatrix[0].size()))){
                    return SIDE_COLLISION;
                }
                if((boardRow >= 0) && (boardRow >= boardMatrix.size())){
                    return BOTTOM_COLLISION;
                }
                if((boardRow >= 0) && boardMatrix[boardRow][boardCol] == 'L'){
                    return SHAPE_COLLISION;
                }
            }
        }
    }
    return NO_COLLISION;
}

s_collisionStatus board::checkCollisionRotate(e_rotate dir){

    if(shape == NULL){
        std::cerr << "No active shape is in play!" << std::endl;
        return NO_COLLISION;
    }

    // Check if any 'A' active element of the
    // shape matrix shifts outside the board matrix
    block tempShape = *shape;
    tempShape.rotate(dir);
    return checkCollision(tempShape);
}

s_collisionStatus board::checkCollisionMove(s_pos newPos){

    if(shape == NULL){
        std::cerr << "No active shape is in play!" << std::endl;
        return NO_COLLISION;
    }

    // Check if any 'A' active element of the
    // shape matrix shifts outside the board matrix
    block tempShape = *shape;
    tempShape.setPosition(newPos);
    return checkCollision(tempShape);
}


void board::moveShape(s_pos newPos){

    if(shape == NULL){
        std::cerr << "No active shape is in play!" << std::endl;
        return;
    }

    shape->setPosition(newPos);
    blockMatrix shapeMatrix = shape->getMatrix();
    // Get coordinates of corners of the shape matrix
    // in relation to the centre position of the shape
    // on the board
    std::pair<s_pos, s_pos> dimension = shape->getDimension();
    s_pos topLeft = dimension.first;
    s_pos bottomRight = dimension.second;

    // Clear the current active shape in the board grid
    for(auto& row : boardMatrix){
        for (auto& elem : row){
            if(elem == 'A')
                elem = ' ';
        }
    }

    // Check if shape is within the grid
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

}

void board::rotateRightShape(){

    if(shape == NULL){
        std::cerr << "No active shape is in play!" << std::endl;
        return;
    }

    s_collisionStatus status = checkCollisionRotate(RIGHT);
    switch(status){
        case NO_COLLISION:
            shape->rotate(RIGHT);                
                break;
        case SIDE_COLLISION:
                break;
        case BOTTOM_COLLISION:
        case SHAPE_COLLISION:
            lockShape();
            break;
        default:
        break;
    }

}
void board::rotateLeftShape(){

    if(shape == NULL){
        std::cerr << "No active shape is in play!" << std::endl;
        return;
    }

    s_collisionStatus status = checkCollisionRotate(LEFT);
    switch(status){
        case NO_COLLISION:
            shape->rotate(LEFT);                
                break;
        case SIDE_COLLISION:
                break;
        case BOTTOM_COLLISION:
        case SHAPE_COLLISION:
            lockShape();
            break;
        default:
        break;
    }
}

bool board::checkLineComplete(){

    bool lineComplete = true;
    for(uint16_t n = boardMatrix.size() - 1; n > 0; n--){
        std::string& row = boardMatrix[n];
        for(auto elem: row){
            if((elem == 'A') || (elem == ' ')){
                lineComplete = false;
                break;
            }
        }
        
        if(lineComplete == true){
            // Whole line is 'L'
            row.assign(row.size(), ' ');
            
            // Drop all elements above this line
            for(uint16_t m = n; m > 0; m--){
                boardMatrix[m] = boardMatrix[m - 1];
            }
        }

    }

    return lineComplete;
}
