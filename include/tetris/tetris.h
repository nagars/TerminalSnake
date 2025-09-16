#ifndef TETRIS_H
#define TETRIS_H

#include <array>
#include "block.h"
#include "frame.h"

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
    std::array <block,7> objects;
    void clearLine();
    void clearScreen();
    void collisionBorder();

    s_size minSize = {.rows = 22, .cols = 24};

    protected:

};

#endif