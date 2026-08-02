# TerminalSnake
My implementation of the original snake game made for terminal on linux

## Requirements
Apart from standard C++ libraries and compiler,
`sudo apt install libsfml-dev`

## To Start
-   Build the project using `make clean && make`. Refer to Frame Rendering section.
-   Run the game using `./snake`

## Frame Rendering
- The game loop is separated from the frame rendering using multi-threading. The FPS can be modified at line 21 of game.cpp
- The game can also be compiled to disable multi-threading and link the frame render with the game loop. Compile with *NOTHREAD* flag. e.g. `make clean && make CPPFLAGS="-DNOTHREAD"`

