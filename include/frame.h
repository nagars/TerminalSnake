#ifndef FRAME_H
#define FRAME_H

#include <sys/ioctl.h>
#include <stdint.h>
#include <fstream>  
#include <string.h>
#include "common.h"

// Frame order
// 0,0 0,1 0,2
// 1,0 1,1 1,2

#define FRAME_BUFFER    1
#define FRAME_BORDER_WIDTH  1

#define FRAME_OFFSET FRAME_BUFFER + FRAME_BORDER_WIDTH

typedef struct {
    uint16_t rows;
    uint16_t cols;
}s_size;


class frame{
    public:
    frame();        // Setup temp file with frame. Get terminal size. 
    frame(char border);
    ~frame();

    // void resizeFrame(void);                 // Check size and rebuild frame in temp file
    void updateFrameElement(char c, uint16_t row, uint16_t col);  // Update the frame in temp file
    void updateFrameRow(const std::string& c, uint16_t row);                  // Update the frame in temp file

    void clearFrame(void);
    void printFrame(void);  // Print frame to terminal
    s_size getFrameSize(void);
    bool isWithinFrame(s_pos pos);

    void enableBorder(char border);
    void enableBorder();
    void disableBorder();

    private:
    s_size termSize;                // size of terminal
    s_size borderedFrameSize;
    std::ofstream frameFileOut;       // OUtput file stream used for frame printing to terminal
    std::ifstream frameFileIn;   // Input file stream used for frame printing to terminal
    s_size getTerminalSize(void);           // Get terminal size
    s_size getBorderedFrameSize(void);  
    void buildBorder(void);
    void writeToFile(const std::string& c, uint16_t pos);
    void writeToFile(const char c, uint16_t pos);

    protected:
    std::string filename = "temp";   
    bool f_setBorder = false;   
    char borderChar = '#'; 

};


#endif