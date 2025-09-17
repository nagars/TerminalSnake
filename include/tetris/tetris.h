#ifndef TETRIS_H
#define TETRIS_H

#include <array>
#include <iostream>
#include <vector>
#include <string>
#include "block.h"
#include "frame.h"

constexpr s_size tetrisSize = {.rows = 22, .cols = 24};   // Size of the board
constexpr s_size infoLeftSize = {.rows = 22, .cols = 18}; // Size of the info

class tetris : public frame{

    public:
    tetris();
    void run();
    void pause();
    void end();

    // Frame uses a unique thread. Dont use a default
    // copy
    tetris(const tetris&) = delete;
    tetris& operator=(const tetris&) = delete;

    private:
    std::array <char,7> shapes = {'I','O','J','T','L','S','Z'};
    //std::vector<std::vector<char>> gameBoard;
    std::array<std::string,tetrisSize.rows> gameBoard;
    std::unique_ptr<block> shape;
    void clearLine();
    void clearScreen();
    void collisionBorder();
    void buildGameBoard();
    void buildInfoBoard();
    void printGame();
    uint8_t randomNumGen();

    protected:

};

#endif