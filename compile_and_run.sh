#!/bin/bash

echo "=== PDB Reader C++ Program Setup ==="
echo ""

# Check if g++ is available
if ! command -v g++ &> /dev/null; then
    echo "Error: g++ compiler not found. Please install g++ first:"
    echo "  sudo apt-get update && sudo apt-get install g++"
    exit 1
fi

echo "Step 1: Compiling the C++ program..."
echo ""

# Method 1: Using Makefile (recommended)
if [ -f "Makefile" ]; then
    echo "Using Makefile to compile..."
    make clean
    make
    if [ $? -eq 0 ]; then
        echo "✓ Compilation successful using Makefile!"
    else
        echo "✗ Compilation failed using Makefile"
        exit 1
    fi
else
    # Method 2: Manual compilation
    echo "Compiling manually..."
    g++ -std=c++17 -Wall -Wextra -O2 -c pdb_reader.cpp -o pdb_reader.o
    g++ -std=c++17 -Wall -Wextra -O2 -c main.cpp -o main.o
    g++ -std=c++17 -Wall -Wextra -O2 -o pdb_reader main.o pdb_reader.o
    
    if [ $? -eq 0 ]; then
        echo "✓ Manual compilation successful!"
    else
        echo "✗ Manual compilation failed"
        exit 1
    fi
fi

echo ""
echo "Step 2: Testing the program with the sample PDB file..."
echo ""

# Check if executable exists
if [ ! -f "./pdb_reader" ]; then
    echo "Error: pdb_reader executable not found"
    exit 1
fi

# Check if sample PDB file exists
if [ ! -f "sample.pdb" ]; then
    echo "Warning: sample.pdb not found. You can download a PDB file from https://www.rcsb.org/"
    echo "Or create your own PDB file."
else
    echo "Running: ./pdb_reader sample.pdb"
    echo ""
    echo "y" | ./pdb_reader sample.pdb
fi

echo ""
echo "=== Program compiled successfully! ==="
echo ""
echo "Usage Instructions:"
echo "  ./pdb_reader <pdb_file>"
echo ""
echo "Examples:"
echo "  ./pdb_reader sample.pdb"
echo "  ./pdb_reader protein.pdb"
echo ""
echo "To download real PDB files, visit: https://www.rcsb.org/"
echo "Example download: curl -o 1HTQ.pdb https://files.rcsb.org/download/1HTQ.pdb"