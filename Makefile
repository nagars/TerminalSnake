# Compiler
CXX = g++
# Compiler flags
# -Iinclude: Tell the compiler to look for header files in the 'include' directory
# -Wall: Enable all warnings
# -std=c++11: Use the C++11 standard
CXXFLAGS = -Iinclude -Wall -std=c++11

# The name of the executable
TARGET = snake

# Find all .cpp files in the src directory
SRCS = $(wildcard src/*.cpp)
# Replace the .cpp extension with .o and put them in the build directory
OBJS = $(patsubst src/%.cpp,build/%.o,$(SRCS))

# The default rule, which is executed when you run 'make'
all: $(TARGET)

# Rule to link the object files into the final executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile a .cpp file into a .o file
# This rule will be used for each source file
build/%.o: src/%.cpp
	@mkdir -p build # Create the build directory if it doesn't exist
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up the project
clean:
	rm -rf build/* $(TARGET)

# Phony targets are not actual files
.PHONY: all clean
