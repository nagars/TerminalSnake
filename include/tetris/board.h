#ifndef BOARD_H
#define BOARD_H

#include <array>
#include "common.h"
#include "frame.h"
#include "block.h"

class board{
    public:
    board();
    board(s_size);

    void createNewShape();
    void shiftLeftShape();
    void shiftRightShape();
    void shiftDownShape();
    void rotateRightShape();
    void rotateLeftShape();

    std::string getRow(uint8_t);
    std::vector<std::string> getMatrix();

    private:
    void lockShape();
    bool checkCollisionBorder();
    bool checkCollisionOtherShape();
    uint8_t randomNumGen();
    std::array <char,7> shapes = {'I','O','J','T','L','S','Z'};
    std::vector<std::string> boardMatrix;
    std::unique_ptr<block> shape;
    s_pos blockStartPos; //= {.x = tetrisBoardSize.cols, .y = -4};///static_cast<int16_t>(2), .y = -4};

    // Matrix to track logic. Not to be confused with size to print
    const s_size tetrisBoardSize = {.rows = 20, .cols = 10};  

    protected:
};

#endif