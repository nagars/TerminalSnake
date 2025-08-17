// Tracks state of game, instantiates other classes and handles printing to terminal

#include "game.h"
#include "common.h"
#include <unistd.h> // For read()
#include <termios.h> // For termios functions
#include <random> // For random number generation facilities


snakeGame::snakeGame(){

// Setup catching keyboard inputs (wasd, space)

// Change terminal settings to a non-blocking read
struct termios attr;
tcgetattr(STDIN_FILENO, &attr); // Get current terminal attributes
attr.c_lflag &= ~(ICANON | ECHO); // Disable canonical mode and echoing
attr.c_cc[VMIN] = 0;        // Read returns immediately (Polling / non-blocking)
attr.c_cc[VTIME] = 0;       // Read has no timeout
tcsetattr(STDIN_FILENO, TCSANOW, &attr); // Apply new terminal attributes

// Setup catching signals (SIGINT | SIGKILL | SIGTERM)

// Setup catching signal (SIGWINCH)

}

    
void snakeGame::run(){

    // Main loop

    // Set the position of the snake
    s_size termSize = getFrameSize();
    s_pos pos;
    pos.y = termSize.rows/2;
    pos.x = termSize.cols/2;
    sneakySnake.setHeadPos(pos);
    sneakySnake.setDirection(DIR_NORTH);

    // Start by placing food
    placeFood();

    while(1){

    char cmd;
    read(STDIN_FILENO, &cmd, 1);

    // Check if food is consumed
    if(true == foodConsumed()){
        // If yes, place new food and extend the snake body
        sneakySnake.extendSnake();
        placeFood();
    }

    clearFrame();
    updateFrameLayout();

    // Print frame to terminal
    printFrame();

    // Check if a collission occurred
    if(true == checkCollision()){
        exit();
        return;
    }
    // get latest command
    // use command to set next position of snake and design
    updateSnake(cmd);
    DEBUG_PRINT("\rCommand Received: %c\n",cmd);


    std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Pause for a short time
    }
}
    
void snakeGame::updateFrameLayout(void){
    
    s_pos pos;

    // Update the frame with new positions    
    updateFrameElement(SNAKE_FOOD, foodPos.y, foodPos.x);

    // Iterate through the snake body and update frame
    std::vector<snakeBodyElement> snakeBody = sneakySnake.getBody();
    for(snakeBodyElement element: snakeBody){
        pos = element.getPos();
        updateFrameElement(element.getDesign(), pos.y, pos.x);
    }

    pos  = sneakySnake.getHeadPos();
    updateFrameElement(sneakySnake.getDesign(), pos.y, pos.x);
}

// void snakeGame::pause(){

// }

void snakeGame::exit(){

    // Flash snake
    for(uint8_t n = 0; n < 5; n++){
        clearFrame();
        printFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Pause for a short time
        updateFrameLayout();
        printFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Pause for a short time
    }

}

bool snakeGame::foodConsumed(void){
    
    s_pos headPos = sneakySnake.getHeadPos();
    if((headPos.x == foodPos.x) && (headPos.y == foodPos.y)){
        return true;
    }

    return false;
}

void snakeGame::placeFood(void){

    s_size frameLimit = getFrameSize();

    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 gen(rd()); // Seed the generator

    // For columns
    std::uniform_int_distribution<> distrib0(1, frameLimit.cols - 1); // Define the distribution
    foodPos.x = distrib0(gen); // Generate the random number

    // For rows
    std::uniform_int_distribution<> distrib1(1, frameLimit.rows - 1); // Define the distribution
    foodPos.y = distrib1(gen); // Generate the random number
}

void snakeGame::updateSnake(char cmd){

    e_DIR dir = sneakySnake.getDirection();
    // update direction based on command
    switch(cmd){
        case 'w':
        case 'W':
            if((dir == DIR_EAST)||(dir == DIR_WEST)){
                sneakySnake.setDirection(DIR_NORTH);
            }
        break;
        
        case 'd':
        case 'D':
            if((dir == DIR_NORTH)||(dir == DIR_SOUTH)){
                sneakySnake.setDirection(DIR_EAST);
            }

        break;
        
        case 'a':
        case 'A':
            if((dir == DIR_NORTH)||(dir == DIR_SOUTH)){
                sneakySnake.setDirection(DIR_WEST);
            }
        break;
        
        case 's':
        case 'S':
            if((dir == DIR_EAST)||(dir == DIR_WEST)){
                sneakySnake.setDirection(DIR_SOUTH);
            }
        break;
        
        default:
        break;
    }

    // move snake by 1
    sneakySnake.moveSnake();

}

bool snakeGame::checkCollision(){

    return sneakySnake.getCollsionStatus();
}

