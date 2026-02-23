# Data Compression via Clique Partitioning

A C++ CLI tool to compress high-dimensional VLSI test patterns using greedy clique partitioning. Handles "Don't Care" bits efficiently and extracts non-overlapping cliques to reduce memory usage and speed up processing.

## Features
- Compress large binary test patterns efficiently
- Handles "Don't Care" (X) bits in vectors
- Greedy clique partitioning for optimized compression
- Fast processing for thousands of high-dimensional vectors

## Requirements
- C++11 or higher
- Standard C++ compiler (g++, clang, etc.)
- Linux, Mac, or Windows

## Usage
### Using Makefile
```bash
# Build the project
make

# Run the tool
./compress input_patterns.txt output_compressed.txt
