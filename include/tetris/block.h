#ifndef BLOCK_H
#define BLOCK_H

#include <array>

typedef enum{
    RIGHT,
    LEFT,
}e_rotate;

//typedef std::array<std::array<char,10>,10> Matrix;
using Matrix = std::array<std::array<char,10>,5>;


class block{

    public:
    block();
    block(char);       // I,O,T,J,L,S,Z

    void setShape(char);
    void rotate(e_rotate);
    Matrix getMatrix(void);

    private:
    //std::vector<std::vector<char>> matrix(10, std::vector<char>(10));
    Matrix matrix;
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