// Main file. Entry of code. Instantiates game class that starts the game.
#include "common.h"
#include "game.h"

#include <thread>  // For std::this_thread::sleep_for
#include <chrono>  // For std::chrono::seconds

int main (int argc, char **argv)
{

    // Init game
    snakeGame game;
    for(uint8_t n = 0; n < 20; n++){
    game.updateFrameElement('>', 12, 22+n);
    game.updateFrameElement('-', 12, 21+n);
    game.updateFrameElement('-', 12, 20+n);
    game.printFrame();
    game.updateFrameElement(' ', 12, 22+n);
    game.updateFrameElement(' ', 12, 21+n);
    game.updateFrameElement(' ', 12, 20+n);

    // sleep for x seconds
    // game start

    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Pause for a short time
    }
    return 0;  
}