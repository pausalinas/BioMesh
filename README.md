# BioMesh
BioMesh is a software tool designed to generate volumetric meshes built with hexahedrons (cubes) by recursive subdivision of space based on an octree algorithm.

## PDB Reader Module

This repository includes a C++ module for reading and parsing PDB (Protein Data Bank) structure files. The module extracts atom information and stores it in STL containers using modern C++ best practices.

### Features

- **PDB File Parsing**: Reads PDB files and extracts ATOM records
- **Smart Pointers**: Uses `std::unique_ptr` to manage memory automatically
- **STL Containers**: Stores atoms in `std::vector` for efficient access
- **Error Handling**: Comprehensive exception handling for file and parsing errors
- **RAII Compliance**: Proper resource management with automatic cleanup
- **Modern C++**: Uses C++14 features and follows best practices

### Building

```bash
mkdir build
cd build
cmake ..
make
```

### Usage

```cpp
#include "PDBReader.h"

try {
    BioMesh::PDBReader reader("structure.pdb");
    reader.parse();
    
    const auto& atoms = reader.getAtoms();
    std::cout << "Parsed " << atoms.size() << " atoms\n";
    
    for (const auto& atom : atoms) {
        std::cout << atom->atom_name << " at (" 
                  << atom->x << ", " << atom->y << ", " << atom->z << ")\n";
    }
} catch (const BioMesh::PDBFileException& e) {
    std::cerr << "File error: " << e.what() << std::endl;
} catch (const BioMesh::PDBParseException& e) {
    std::cerr << "Parse error: " << e.what() << std::endl;
}
```

### Example

Run the included example:
```bash
./pdb_example examples/sample.pdb
```
