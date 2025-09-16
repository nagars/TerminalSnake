#include "tetris.h"

tetris::tetris() : frame(){

    // Check if the terminal size is large enough
    s_size frameSize = getFrameSize();
    if((frameSize.cols < minSize.cols)||(frameSize.rows < minSize.rows)){
        std::cerr << "Error: Terminal size is too small!\n" << std::endl;
        exit(0);
    }

    // Build default screen
    clearFrame();

    // Build the border
    buildBorder();
};

void tetris::run(){

}

void tetris::pause(){


}
void tetris::end(){


}

void tetris::clearLine(){


}

void tetris::clearScreen(){


}

void tetris::collisionBorder(){


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
void tetris::buildBorder(){

    s_size frameSize = getFrameSize();
    uint16_t offset = 0;

    std::string row(frameSize.cols,' ');
    // If there are odd number of cols,
    // adjust size of layout so all blocks fit
    if(frameSize.cols % 2){
        offset = 1;
    }

    // Each alternate element should have a period
    for(uint16_t n = 1; n < row.size(); n+=2){
        row[n] = '.';
    }

    // Build border (verticals)
    row[0] = '<';
    row[1] = '!';
    row[frameSize.cols - offset - 1] = '>';
    row[frameSize.cols - offset - 2] = '!';

    for(uint16_t n = 0; n < frameSize.rows - 2; n++){
        updateFrameRow(row, n);
    }

    // Build base at bottom
    row.assign(frameSize.cols,'=');
    row[0] = '<';
    row[1] = '!';
    row[frameSize.cols - offset - 1] = '>';
    row[frameSize.cols - offset - 2] = '!';
    row[frameSize.cols - offset] = ' ';
    updateFrameRow(row,frameSize.rows - 2);

    row.assign(frameSize.cols, ' ');
    for(uint16_t n = 2; n < frameSize.cols - offset; n++){
        //row.replace(n, 2, "\\/");
        if(n % 2)
            row[n] = '/';
        else{
            row[n] = '\\';
        }
    }
    updateFrameRow(row,frameSize.rows - 1);

    printFrame();
}