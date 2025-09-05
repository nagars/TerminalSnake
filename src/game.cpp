// Tracks state of game, instantiates other classes and handles printing to terminal

#include "game.h"
#include "common.h"
#include <iostream>
#include <unistd.h> // For read()
#include <termios.h> // For termios functions
#include <random> // For random number generation facilities

#define MIN_SIZE_ROW    15
#define MIN_SIZE_COL    20

#define SPEED_UP 5

snakeGame::snakeGame() : frame((fps)10){

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
    enableBorder();

    // Get the size of the terminal
    s_size termSize = getFrameSize();
    DEBUG_PRINT("\rTerminal Size: %d,%d\n",termSize.cols, termSize.rows);

    // Check if terminal size is larger than minimum required.
    try{
        if((termSize.cols < MIN_SIZE_COL) || (termSize.rows < MIN_SIZE_ROW)){
            throw(std::runtime_error("Terminal size is too small!"));
        }
    }catch(const std::runtime_error& e){
            std::cerr << "Runtime error caught: " << e.what() << std::endl;
            return;
    }

    // Set the position of the snake in the centre of the screen
    s_pos pos;
    pos.y = termSize.rows/2;
    pos.x = termSize.cols/2;
    sneakySnake.setHeadPos(pos);
    sneakySnake.setDirection(DIR_NORTH);

    // Start by placing food
    placeFood();

    // Store command from terminal
    char cmd;

    // total food consumed
    uint16_t numFoodConsumed = 0;
    
    // Sleep delay
    uint16_t sleep_ms = 100; 

    #ifdef DEBUG
    addDebugInfo("Position X",pos.x);
    addDebugInfo("Position Y",pos.y);
    addDebugInfo("Command", (uint16_t)0);
    #endif
    addDebugInfo("Current Score", numFoodConsumed);

    while(1){

        // read command from terminal
        read(STDIN_FILENO, &cmd, 1);

        // Check if food is consumed
        if(true == foodConsumed()){
            // If yes, place new food and extend the snake body
            sneakySnake.extendSnake();
            placeFood();
            numFoodConsumed += 1;
            
            // Increase speed of snake after every 5 food consumed
            if(numFoodConsumed % 5 == 0)
                sleep_ms -= SPEED_UP;
        }

        // Check if a collision occurred
        if(true == checkCollision()){
            endGame();
            return;
        }

        // Check border collision
        if(true == checkBorderCollision()){
            endGame();
            return;
        }

        clearFrame();
        updateFrameLayout();

        // Print frame to terminal
        //printFrame();

        // get latest command
        // use command to set next position of snake and design
        updateSnake(cmd);
        
        // Update debug info
        #ifdef DEBUG
        s_pos pos = sneakySnake.getHeadPos();
        addDebugInfo("Position X",pos.x);
        addDebugInfo("Position Y",pos.y);
        addDebugInfo("Command", cmd);
        #endif
        
        addDebugInfo("Current Score", numFoodConsumed);


        std::this_thread::sleep_for(std::chrono::milliseconds(sleep_ms)); // Pause for a short time
    }
}
    
void snakeGame::updateFrameLayout(void){
    
    s_pos pos;

    lockFrameRenderer();
    
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

    releaseFrameRenderer();
}


void snakeGame::endGame(){

    // Flash snake
    for(uint8_t n = 0; n < 5; n++){
        clearFrame();
        //printFrame();
        std::this_thread::sleep_for(std::chrono::milliseconds(300)); // Pause for a short time
        updateFrameLayout();
        //printFrame();
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
    std::uniform_int_distribution<> distrib0(FRAME_OFFSET, frameLimit.cols - 1); // Define the distribution
    foodPos.x = distrib0(gen); // Generate the random number

    // For rows
    std::uniform_int_distribution<> distrib1(FRAME_OFFSET, frameLimit.rows - 1); // Define the distribution
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

bool snakeGame::checkBorderCollision(){

    // Predict next position of snake head
    // and check if border collision will occur
    s_pos snakePos = sneakySnake.getHeadPos();
    e_DIR direction = sneakySnake.getDirection();

    switch(direction){
        case DIR_NORTH:
        snakePos.y -= 1;
        break;
        case DIR_SOUTH:
        snakePos.y += 1;
        break;
        case DIR_EAST:
        snakePos.x += 1;
        break;
        case DIR_WEST:
        snakePos.x -= 1;
        break;
        default:
        break;
    }

    return !isWithinFrame(snakePos);
}
