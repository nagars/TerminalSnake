#ifndef FRAME_H
#define FRAME_H

#include <sys/ioctl.h>
#include <stdint.h>
#include <fstream>  
#include <string.h>
#include <mutex>
#include <atomic>
#include <unordered_map>
#include <vector>
#include "common.h"

// Frame order
// 0,0 0,1 0,2
// 1,0 1,1 1,2

// Tracks the size of the frame / terminal
typedef struct {
    uint16_t rows;
    uint16_t cols;
}s_size;

typedef uint16_t fps;

class frame{
    public:
    frame();        
    frame(char border);

    // Initalise with multi-threaded renderer
    frame(fps fps);
    frame(fps fps, char border);

    ~frame();

    // Update temporary file containg the frame
    void updateFrameElement(char c, int16_t row, int16_t col); 
    void updateFrameRow(const std::string& c, int16_t row);   
    void clearFrame(void);              

    // Write to terminal
    void printFrame(void);  

    s_size getFrameSize(void);
    bool isWithinFrame(s_pos pos);

    // Setter / Getters
    void enableBorder(char border);
    void enableBorder();
    void disableBorder();

    // Mutex APIs
    void lockFrameRenderer();
    void releaseFrameRenderer();

    // Debug APIs
    void addDebugInfo(const std::string& literal, int16_t value);
    void addDebugInfo(const std::string& literal, char value);

    // A class with a std::thread member cannot be copied
    frame(const frame&) = delete;
    frame& operator=(const frame&) = delete;

    private:
    bool f_setBorder = false;       // Tracks if a frame border has to be created 
    std::atomic<bool> f_renderActive{false};    // Tracks if the frame printing is being 
                                                // done in a separate thread
    bool f_renderLockAcquired = false;  // Tracks if the render mutex is locked
    char borderChar = '#';          // Character to be used to build the border
    std::mutex renderMutex;         // Mutex to synchronise renderer thread with main thread
    fps frameRate = 0;              // Set FPS
    std::thread frameRenderThread;  // Thread used to print to terminal
    std::unordered_map<std::string, int16_t> debugInfoMap; // Stores the debug info to be printed
    std::vector<std::vector<char>> displayMatrix;

    // Get the frame size
    s_size getTerminalSize(void);       
    s_size getBorderedFrameSize(void);  

    // Write a border of the frame to the temp file
    void buildBorder(void);

    // Write to temp file
    void writeToFile(const std::string& c, uint16_t pos);
    void writeToFile(const char c, uint16_t pos);

    // For multi-threaded intialisation, adjusts the timing of the renderer thread
    void updateFPS(fps fps);
    void frameRenderWorker(void);
    void printFrameWorker(void);
    void clearFrameWorker(void);

    s_size resizeDisplayMatrix(void);


    protected:
    std::string filename = "temp";  // Temporary file name 

};

#endif