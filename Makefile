# Compiler
CXX = g++ -g
# Compiler flags
# -Iinclude: Tell the compiler to look for header files in the 'include' directory
# -Wall: Enable all warnings
# -std=c++11: Use the C++11 standard
CXXFLAGS = -Iinclude -Iinclude/common -Iinclude/snake -Iinclude/tetris -Wall -std=c++11 

# Define linker flags, including ncurses
LDFLAGS = -lsfml-audio

# The name of the executable
TARGET = game

# Find all .cpp files in the src directory and its subdirectories
SRCS = $(shell find src -name '*.cpp')
# Replace the src/%.cpp with build/%.o to create the object file list
OBJS = $(patsubst src/%.cpp,build/%.o,$(SRCS))

# The default rule, which is executed when you run 'make'
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Rule to compile a .cpp file into a .o file
# This rule will be used for each source file
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up the project
clean:
	rm -rf build/* $(TARGET)

# Phony targets are not actual files
.PHONY: all clean rebuild

rebuild: clean all
