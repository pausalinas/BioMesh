# PDB File Reader in C++

A C++ program to read and parse Protein Data Bank (PDB) files, which are standard format files used to store 3D molecular structure data for proteins, nucleic acids, and other biological macromolecules.

## Features

- **Complete PDB Parser**: Reads ATOM and HETATM records according to PDB format specification
- **Comprehensive Data Structures**: Stores all essential atom information including coordinates, occupancy, temperature factors, etc.
- **File Summary**: Displays PDB metadata (title, date, PDB ID) and statistical information
- **Atom Details**: Shows detailed atom information in a formatted table
- **Error Handling**: Robust parsing with graceful error handling for malformed files
- **Cross-platform**: Compatible with Linux, macOS, and Windows

## Files Structure

```
├── pdb_reader.h          # Header file with data structures and function declarations
├── pdb_reader.cpp        # Implementation of PDB parsing functions
├── main.cpp              # Main program with user interface
├── Makefile              # Build configuration
├── compile_and_run.sh    # Automated compilation and testing script
├── sample.pdb            # Sample PDB file for testing
└── README_PDB.md         # This documentation file
```

## Data Structures

### Atom Structure
The `Atom` struct contains all fields from the PDB ATOM record:
- Serial number, atom name, residue information
- 3D coordinates (x, y, z) in Angstroms
- Occupancy and temperature factor
- Element symbol and charge

### PDBData Structure
The `PDBData` struct represents a complete PDB file:
- Metadata (title, date, PDB ID)
- Vector of all atoms
- Methods for display and analysis

## Compilation Methods

### Method 1: Using Makefile (Recommended)
```bash
make clean    # Clean previous builds
make          # Compile the program
```

### Method 2: Manual Compilation
```bash
# Compile object files
g++ -std=c++17 -Wall -Wextra -O2 -c pdb_reader.cpp -o pdb_reader.o
g++ -std=c++17 -Wall -Wextra -O2 -c main.cpp -o main.o

# Link to create executable
g++ -std=c++17 -Wall -Wextra -O2 -o pdb_reader main.o pdb_reader.o
```

### Method 3: Single Command Compilation
```bash
g++ -std=c++17 -Wall -Wextra -O2 main.cpp pdb_reader.cpp -o pdb_reader
```

### Method 4: Automated Script
```bash
./compile_and_run.sh
```

## Usage

### Basic Usage
```bash
./pdb_reader <pdb_file>
```

### Examples
```bash
# Test with included sample file
./pdb_reader sample.pdb

# Download and test with real PDB file
curl -o 1HTQ.pdb https://files.rcsb.org/download/1HTQ.pdb
./pdb_reader 1HTQ.pdb
```

## Sample Output

```
Successfully read PDB file: sample.pdb
Total atoms loaded: 26

=== PDB File Summary ===
PDB ID: 1HTQ
Date: 20-MAY-93
Title: STRUCTURE OF THE LARGE FRAGMENT OF THERMUS AQUATICUS DNA POLYMERASE I
Number of atoms: 26
Number of chains: 1
Coordinate ranges:
  X: 6.55 to 20.15 Å
  Y: 13.36 to 19.57 Å
  Z: 18.24 to 25.05 Å

Would you like to see detailed atom information? (y/n): y

=== Atom Details ===
Serial Name Res Ch ResNo         X         Y         Z     Occ   TempF Elem
--------------------------------------------------------------------------------
     1    N ALA  A     1    20.154    16.967    22.084     1.00   25.00    N
     2   CA ALA  A     1    19.030    16.101    21.671     1.00   25.00    C
     3    C ALA  A     1    17.664    16.849    21.548     1.00   25.00    C
     ...
```

## PDB Format Information

The Protein Data Bank format is a standard text-based format for molecular structure data. Key record types supported:

- **HEADER**: Basic information about the structure
- **TITLE**: Descriptive title of the structure
- **ATOM**: Atomic coordinate data for standard residues
- **HETATM**: Atomic coordinate data for non-standard groups (water, ligands, etc.)

## Requirements

- **Compiler**: g++ with C++17 support
- **Operating System**: Linux, macOS, or Windows with appropriate compiler
- **Dependencies**: Standard C++ library only (no external dependencies)

### Installing g++ on Linux
```bash
# Ubuntu/Debian
sudo apt-get update && sudo apt-get install g++

# CentOS/RHEL
sudo yum install gcc-c++

# Arch Linux
sudo pacman -S gcc
```

## Getting PDB Files

You can obtain PDB files from several sources:

1. **RCSB Protein Data Bank**: https://www.rcsb.org/
   ```bash
   curl -o 1HTQ.pdb https://files.rcsb.org/download/1HTQ.pdb
   ```

2. **Popular test structures**:
   - 1HTQ: DNA Polymerase I
   - 1A2S: Lysozyme
   - 1BNA: DNA Double Helix

## Troubleshooting

### Compilation Issues
- Ensure g++ is installed and supports C++17
- Check that all source files are in the same directory
- Verify file permissions for the compile script

### Runtime Issues
- Ensure the PDB file exists and is readable
- Check that the PDB file follows standard format
- Large files may take time to process

### Common Errors
- "Cannot open file": Check file path and permissions
- Compilation errors: Ensure C++17 support is available

## Future Enhancements

Potential improvements for this PDB reader:
- Support for additional PDB record types (SEQRES, SSBOND, etc.)
- Distance and angle calculations between atoms
- Export to different file formats
- Visualization capabilities
- Performance optimization for large structures
- GUI interface

## Contributing

Feel free to enhance this PDB reader by:
- Adding more PDB record type support
- Improving error handling
- Adding analysis functions
- Optimizing performance

## License

This project is open source. Feel free to use, modify, and distribute as needed.