# BioMesh PDB File Reader

BioMesh is a software tool designed to generate volumetric meshes built with hexahedrons (cubes) by recursive subdivision of space based on an octree algorithm. 

This repository now includes a C++ PDB (Protein Data Bank) file reader that can open, parse, and display information from PDB files.

## Features

- Read and parse PDB files according to the standard format
- Extract ATOM and HETATM records with atomic coordinates
- Display file summary with coordinate bounds
- Show detailed atom information
- Support for manual compilation and execution from terminal

## Files

- `pdb_reader.h` - Header file with PDB data structures and class definitions
- `pdb_reader.cpp` - Implementation of the PDB reader functionality  
- `main.cpp` - Main program demonstrating the PDB reader usage
- `Makefile` - Build configuration for easy compilation
- `sample.pdb` - Sample PDB file for testing

## Compilation

### Using Makefile (Recommended)

```bash
# Build the program
make

# Create a sample PDB file for testing
make sample

# Clean build artifacts
make clean

# Show help with all available targets
make help
```

### Manual Compilation

```bash
g++ -std=c++11 -Wall -Wextra -O2 -o pdb_reader main.cpp pdb_reader.cpp
```

### Requirements

- C++11 compatible compiler (g++, clang++, etc.)
- Standard C++ libraries (iostream, fstream, string, vector, sstream)

## Usage

### Command Line

```bash
./pdb_reader <pdb_file>
```

### Example

```bash
# Create a sample PDB file
make sample

# Run the program with the sample file
./pdb_reader sample.pdb
```

The program will:
1. Display a welcome message
2. Open and parse the specified PDB file
3. Show a summary with file information and coordinate bounds
4. Ask if you want to see detailed atom information
5. Display the requested information

### Sample Output

```
=== BioMesh PDB File Reader ===
A tool to read and parse PDB (Protein Data Bank) files

Attempting to open file: sample.pdb
Successfully loaded PDB file: sample.pdb
Number of atoms parsed: 10

=== PDB FILE SUMMARY ===
Filename: sample.pdb
Header: HEADER    SAMPLE PROTEIN                          01-JAN-24   XXXX
Total atoms: 10
Coordinate bounds:
  X: 14.089 to 20.154
  Y: 15.411 to 18.527
  Z: 8.189 to 10.857

Would you like to see detailed atom information? (y/n): y

=== ATOM RECORDS ===
Type  Num   Name  Res ChResN X         Y         Z         
------------------------------------------------------------
ATOM  1     N     ALA A 1    20.154    16.967    10.568    
ATOM  2     CA    ALA A 1    19.030    16.807    9.528     
ATOM  3     C     ALA A 1    17.715    17.354    10.121    
...
```

## PDB Format Support

The reader supports the following PDB record types:
- `HEADER` - File identification and classification
- `ATOM` - Atomic coordinate records for standard amino acids and nucleotides
- `HETATM` - Atomic coordinate records for hetero atoms (ligands, water, etc.)
- `END` - End of file marker

Each atom record includes:
- Atom serial number and name
- Residue name and number
- Chain identifier
- 3D coordinates (x, y, z)
- Occupancy and temperature factor
- Element symbol and charge (if available)

## Error Handling

The program includes comprehensive error handling for:
- File not found or cannot be opened
- Invalid PDB format lines
- Missing required fields
- Coordinate parsing errors

## Contributing

This is part of the BioMesh project. Feel free to contribute improvements or report issues.

## License

See the LICENSE file for licensing information.
