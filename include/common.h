#ifndef COMMON_H
#define COMMON_H

#include <thread>  // For std::this_thread::sleep_for
#include <chrono>  // For std::chrono::seconds
#include <cstdio>

#define DEBUG

#ifdef DEBUG
    #define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
    #define DEBUG_PRINT(...) ((void)0)
#endif

// Sets the position in the frame matrix
typedef struct {
    int16_t x;         // X axis (Cols)
    int16_t y;         // Y axis (rows)
}s_pos;


#endif