# BioMesh

BioMesh is a software tool designed to generate volumetric meshes built with hexahedrons (cubes) by recursive subdivision of space based on an octree algorithm.

## Features

### Molecular Hierarchy System
- **Two-level hierarchy**: Organize atoms into molecules based on chain IDs
- **Chain-based organization**: Automatic detection and separation of molecular chains
- **Backward compatibility**: Legacy flat atom list access maintained

### Enhanced Atom Properties
- **Chemical elements**: Automatic detection from PDB atom names
- **Atomic radius**: Assigned from built-in specification table
- **Atomic mass**: Provided for all common biological elements
- **PDB compatibility**: Full support for standard PDB file format

### Atomic Specification System
- **Built-in elements**: C, N, O, S, P, H, FE, ZN, MG, CA, K, NA, CL
- **Customizable**: Add your own element specifications
- **Automatic assignment**: Properties applied during PDB parsing

### Builder Pattern
- **Fluent interface**: Easy atom construction with method chaining
- **Auto-detection**: Smart element detection from atom names
- **Flexible**: Support for both explicit and automatic property assignment

### Bounding Domain Calculations
- **Lazy evaluation**: Computed on demand and cached for performance
- **Multiple metrics**: Bounding box, center point, bounding sphere radius
- **Point testing**: Check if coordinates fall within molecular bounds

## Quick Start

### Building
```bash
mkdir build && cd build
cmake ..
make
```

### Usage Example
```cpp
#include "biomesh/biomesh.h"
using namespace biomesh;

// Create PDB reader with default atomic specifications
auto spec_table = createDefaultSpecTable();
PDBReader reader(spec_table);

// Parse PDB content
reader.readFile("protein.pdb");

// Access molecules by chain
const auto* chainA = reader.getMolecule("A");
if (chainA) {
    std::cout << "Chain A has " << chainA->getAtomCount() << " atoms" << std::endl;
    
    // Get bounding information
    const auto& bbox = chainA->getBoundingBox();
    double cx, cy, cz;
    chainA->getCenter(cx, cy, cz);
    double radius = chainA->getBoundingSphereRadius();
}

// Build atoms with enhanced properties
AtomBuilder builder(spec_table);
Atom atom = builder.setSerial(1)
                   .setAtomName("CA")
                   .setResidueName("ALA")
                   .setChainId("A")
                   .setCoordinates(1.0, 2.0, 3.0)
                   .autoDetectElement()
                   .build();
```

## Testing
```bash
# Build and run tests
cd build
g++ -std=c++17 -I../include ../tests/test_core.cpp libbiomesh.a -o test_core
./test_core
```

## Requirements
- C++17 compatible compiler
- CMake 3.16 or later

## License
See LICENSE file for details.
