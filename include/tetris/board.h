#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "common.h"
#include "frame.h"
#include "block.h"

typedef enum{
    NO_COLLISION,       
    SIDE_COLLISION,     // Collision with side of board
    BOTTOM_COLLISION,   // Collision with bottom of board
    SHAPE_COLLISION,    // Collision with another shape
}s_collisionStatus;

const std::array <char,7> shapes = {'I','O','J','T','L','S','Z'};


class tetrisBoard{
    public:
    tetrisBoard();
    tetrisBoard(s_size);

    void createNewShape();          // Generates random shape
    void createNewShape(uint16_t);  // Generates shape based on number provided    
    uint8_t randomNumGen();
    void shiftLeftShape();
    void shiftRightShape();
    void shiftDownShape();
    void rotateRightShape();
    void rotateLeftShape();
    bool getShapeActiveStatus();
    bool checkLineComplete();

    std::string getRow(uint8_t);
    std::vector<std::string> getMatrix();

    private:
    void moveShape(s_pos);
    void lockShape();
    s_collisionStatus checkCollision(block&);
    s_collisionStatus checkCollisionRotate(e_rotate);
    s_collisionStatus checkCollisionMove(s_pos);
    std::vector<std::string> boardMatrix;
    std::unique_ptr<block> shape;
    s_pos blockStartPos; 

    // Matrix to track logic. Ignores boundary characters. Not to be confused with size to print
    const s_size tetrisBoardSize = {.rows = 20, .cols = 10};  

    protected:
};


class infoBoard{

    public:
    infoBoard();

    void setLevel(uint16_t);
    void setScore(uint16_t);
    void setTime(uint16_t minutes, uint16_t seconds);
    void setFullLines(uint16_t);
    void setNextShape(block);

    std::string getRow(uint8_t);
    std::vector<std::string> getMatrix();
    
    private:
    std::vector<std::string> boardMatrix;

    const s_size infoBoardSize = {.rows = 15, .cols = 18};
    const s_pos fullLinePos = {.x = 13, .y = 0};
    const s_pos levelPos = {.x = 8, .y = 1};
    const s_pos scorePos = {.x = 8, .y = 2};
    const s_pos timePos = {.x = 7, .y = 3};
    const s_pos shapePos = {.x = 13, .y = 8}; 

    protected:
};

#endif