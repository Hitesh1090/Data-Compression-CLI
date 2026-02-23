# Compiler
CXX = g++

# Compiler flags (-O3 for high performance optimization)
CXXFLAGS = -std=c++11 -O3 -Wall

# Target executable name
TARGET = vector_solver

# Source file
SRC = data_compress.cpp

# Default target
all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Clean target to remove the executable
clean:
	rm -f $(TARGET)