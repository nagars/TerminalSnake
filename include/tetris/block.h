#ifndef BLOCK_H
#define BLOCK_H

#include <array>
#include <string>
#include "common.h"

typedef enum{
    RIGHT,
    LEFT,
}e_rotate;

//typedef std::array<std::array<char,10>,10> Matrix;
//using blockMatrix = std::array<std::array<char,10>,5>;
using blockMatrix = std::array<std::string,5>;


class block{

    public:
    block();
    block(char);       // I,O,T,J,L,S,Z

    void setShape(char);
    void rotate(e_rotate);
    blockMatrix getMatrix(void);
    void setPosition(s_pos);
    s_pos getPosition(void);
    std::pair<s_pos, s_pos> getDimension();

    private:
    //std::vector<std::vector<char>> matrix(10, std::vector<char>(10));
    blockMatrix matrix;
    s_pos position = {.x = 0, .y = 0};
    void matrixTranspose();
    void rowReverse();
    void colReverse();

    void createI();
    void createO();
    void createT();
    void createJ();
    void createL();
    void createS();
    void createZ();

    const std::array<char,2> element = {'[',']'};   // Definition of each element

    protected:
};

#endif