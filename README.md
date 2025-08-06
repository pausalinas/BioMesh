# BioMesh
BioMesh is a software tool designed to generate volumetric meshes built with hexahedrons (cubes) by recursive subdivision of space based on an octree algorithm.

## PDB Example

This repository includes `pdb_example.cpp`, a demonstration program that parses PDB (Protein Data Bank) files and displays enhanced results showing molecular structure data.

### Features

- **Total atom count display**: Shows the total number of atoms parsed from the PDB file
- **Enhanced atom display**: Shows first 5 and last 5 atoms with full details
- **Better coordinate formatting**: Improved readability with aligned columns and 2 decimal places
- **Smart separator**: Indicates atoms in between when more than 10 atoms are present
- **Robust error handling**: RAII-based design with comprehensive error checking
- **Multiple build systems**: Support for both Make and CMake

### Building

#### Using Make:
```bash
make
```

#### Using CMake:
```bash
mkdir build && cd build
cmake ..
make
```

### Usage

```bash
# Run with automatic sample file generation
./pdb_example

# Run with your own PDB file
./pdb_example your_file.pdb
```

### Example Output

```
============================================================
           PDB PARSING RESULTS
============================================================
File: sample.pdb
Total atoms parsed: 15
------------------------------------------------------------

First 5 atoms:
#   SerialName Res ChResN X         Y         Z         Elm 
------------------------------------------------------------
1   1      N   ALA A 1         20.15     16.97     27.46 N  
2   2      CA  ALA A 1         19.03     16.10     27.09 C  
3   3      C   ALA A 1         18.46     16.63     25.79 C  
4   4      O   ALA A 1         18.82     17.69     25.27 O  
5   5      CB  ALA A 1         17.95     16.01     28.18 C  

............................................................
... 5 atoms in between ...
............................................................

Last 5 atoms:
#   SerialName Res ChResN X         Y         Z         Elm 
------------------------------------------------------------
11  11     OG1 THR A 2         18.68     16.90     22.81 O  
12  12     CG2 THR A 2         16.72     16.34     21.57 C  
13  13     N   VAL A 3         14.69     15.84     22.88 N  
14  14     CA  VAL A 3         13.39     15.18     22.62 C  
15  15     C   VAL A 3         12.63     15.79     21.45 C  

============================================================
```
