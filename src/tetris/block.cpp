#include "block.h"
#include <stdexcept>
#include <iostream>
#include <stdint.h>

block::block(){

}

block::block(char shape){

    setShape(shape);
}

void block::setShape(char shape){

    // Clear block array
    for(uint8_t n = 0; n < matrix.size(); n++)
       // matrix[n].fill(' ');
       matrix[n].assign(5,' ');

    if((shape == 'i') || (shape == 'I')){
        createI();
    }else     if((shape == 'o') || (shape == 'O')){
        createO();
    }else     if((shape == 't') || (shape == 'T')){
        createT();
    }else    if((shape == 'j') || (shape == 'J')){
        createJ();
    }else    if((shape == 'l') || (shape == 'L')){
        createL();
    }else    if((shape == 's') || (shape == 'S')){
        createS();
    }else    if((shape == 'z') || (shape == 'Z')){
        createZ();
    }else{
        try{
            throw("Invalid tetromino request");
        }catch(const std::runtime_error& e){
        std::cerr << "Exception caught in file: " << __FILE__ 
                 << ", line: " << __LINE__ 
                 << ". Error: " << e.what() << std::endl;
        }
    }

}

blockMatrix block::getMatrix(){
    return matrix;
}

std::pair<s_pos, s_pos> block::getDimension(){

    // Return lefttop corner and rightbottom corner
    s_pos leftTop = {.x = position.x - matrix[0].size()/2, .y = position.y - matrix.size()/2};
    s_pos rightBottom = {.x = position.x + matrix[0].size()/2, .y = position.y + matrix.size()/2};

    std::pair <s_pos, s_pos> dimension (leftTop,rightBottom);
    return dimension;

}

void block::rotate(e_rotate rotate){
    if(rotate == LEFT){
        matrixTranspose();
        colReverse();
    }else if(rotate == RIGHT){
        matrixTranspose();
        rowReverse();
    }else{
        try{
            throw("Invalid tetromino rotate");
        }catch(const std::runtime_error& e){
        std::cerr << "Exception caught in file: " << __FILE__ 
                 << ", line: " << __LINE__ 
                 << ". Error: " << e.what() << std::endl;
        }
    }
}

void block::matrixTranspose(){

}

void block::rowReverse(){

}

void block::colReverse(){

}

void block::setPosition(s_pos pos){

    position = pos;
}

s_pos block::getPosition(void){

    return position;
}


/**
 * []
 * []
 * []
 * []
 */
void block::createI(){

    // matrix[1][5] = '[';
    // matrix[1][6] = ']';
    // matrix[2][5] = '[';
    // matrix[2][6] = ']';
    // matrix[3][5] = '[';
    // matrix[3][6] = ']';
    // matrix[4][5] = '[';
    // matrix[4][6] = ']';

    matrix[0][2] = '*';
    matrix[1][2] = '*';
    matrix[2][2] = '*';
    matrix[3][2] = '*'; 
}

/**
 *  []
 *  []
 *[][]
 */
void block::createJ(){

    // matrix[1][5] = '[';
    // matrix[1][6] = ']';
    // matrix[2][5] = '[';
    // matrix[2][6] = ']';
    // matrix[3][5] = '[';
    // matrix[3][6] = ']';
    // matrix[4][3] = '[';
    // matrix[4][4] = ']';

    matrix[0][2] = '*';
    matrix[1][2] = '*';
    matrix[2][2] = '*';
    matrix[2][1] = '*'; 
}

/**
 * []
 * []
 * [][]
 */
void block::createL(){
    
    // matrix[1][3] = '[';
    // matrix[1][4] = ']';
    // matrix[2][3] = '[';
    // matrix[2][4] = ']';
    // matrix[3][3] = '[';
    // matrix[3][4] = ']';
    // matrix[4][5] = '[';
    // matrix[4][6] = ']';

    matrix[0][2] = '*';
    matrix[1][2] = '*';
    matrix[2][2] = '*';
    matrix[2][3] = '*'; 
}

/**
 * [][]
 * [][]
 */
void block::createO(){
    
    // matrix[2][5] = '[';
    // matrix[2][6] = ']';
    // matrix[2][7] = '[';
    // matrix[2][8] = ']';
    // matrix[3][5] = '[';
    // matrix[3][6] = ']';
    // matrix[3][7] = '[';
    // matrix[3][8] = ']';

    matrix[2][3] = '*';
    matrix[3][3] = '*';
    matrix[2][4] = '*';
    matrix[3][4] = '*'; 
}

/**
 *   [][]
 * [][]
 */
void block::createS(){

    // matrix[2][5] = '[';
    // matrix[2][6] = ']';
    // matrix[2][7] = '[';
    // matrix[2][8] = ']';
    // matrix[3][5] = '[';
    // matrix[3][6] = ']';
    // matrix[3][3] = '[';
    // matrix[3][4] = ']';

    matrix[2][2] = '*';
    matrix[2][3] = '*';
    matrix[1][3] = '*';
    matrix[1][4] = '*'; 
}

/**
 * [][][]
 *   []
 */
void block::createT(){

//     matrix[2][3] = '[';
//     matrix[2][4] = ']';
//     matrix[2][5] = '[';
//     matrix[2][6] = ']';
//     matrix[2][7] = '[';
//     matrix[2][8] = ']';
//     matrix[3][5] = '[';
//     matrix[3][6] = ']';

    matrix[2][1] = '*';
    matrix[2][2] = '*';
    matrix[2][3] = '*';
    matrix[3][2] = '*'; 

 }

/**
 * [][]
 *   [][]
 */
void block::createZ(){

    // matrix[2][3] = '[';
    // matrix[2][4] = ']';
    // matrix[2][5] = '[';
    // matrix[2][6] = ']';
    // matrix[3][5] = '[';
    // matrix[3][6] = ']';
    // matrix[3][7] = '[';
    // matrix[3][8] = ']';

    matrix[1][1] = '*';
    matrix[1][2] = '*';
    matrix[2][2] = '*';
    matrix[2][3] = '*'; 
}