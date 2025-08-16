// Main file. Entry of code. Instantiates game class that starts the game.
#include "common.h"
#include "game.h"

int main (int argc, char **argv)
{

    // Init game
    snakeGame game;
    std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Pause for a short time

    // Start the game
    game.run();

    std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Pause for a short time
    
    return 1;  
}