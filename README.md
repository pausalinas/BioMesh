# BioMesh

BioMesh is a software tool designed to generate volumetric meshes built with hexahedrons (cubes) by recursive subdivision of space based on an octree algorithm. The library provides enhanced atomic modeling capabilities with comprehensive atomic properties management.

## Features

### Enhanced Atom Architecture
- **Multiple constructors**: Support for different initialization scenarios
- **Atomic properties**: Automatic assignment of atomic radius and mass
- **Builder pattern**: `AtomBuilder` class for seamless property lookup
- **Comprehensive element database**: Pre-configured properties for common biological elements
- **Error handling**: Robust validation for unknown elements

### Key Components

#### Atom Class
The `Atom` class represents atoms with the following properties:
- 3D coordinates (x, y, z)
- Chemical element symbol
- Atomic radius (Å)
- Atomic mass (Da)

**Constructors:**
```cpp
Atom();                                                    // Default constructor
Atom(double x, double y, double z, const std::string& element);  // Coordinates + element
Atom(const std::string& element);                         // Element only
Atom(const std::string& element, double radius);          // Element + radius
Atom(const std::string& element, double radius, double mass);    // Element + radius + mass
```

#### AtomBuilder Class
The `AtomBuilder` class provides automatic property assignment using a comprehensive atomic specification database:

```cpp
BioMesh::AtomBuilder builder;
std::vector<BioMesh::Atom> parsedAtoms = /* atoms from PDB parser */;
auto enhancedAtoms = builder.buildAtoms(parsedAtoms);  // Automatically assigns radius/mass
```

#### Supported Elements
Pre-configured atomic properties for:
- Common biological elements: H, C, N, O, P, S
- Metal ions: Na, Mg, K, Ca, Fe, Zn, Cu, Mn, Co, Ni, Mo
- Other elements: Cl, Se, I

## Building

### Requirements
- CMake 3.10+
- C++17 compatible compiler
- GoogleTest (optional, for unit tests)

### Build Instructions
```bash
mkdir build && cd build
cmake ..
make -j4
```

### Running Tests
```bash
# Direct execution
./atom_tests

# Using CTest
ctest --verbose
```

### Running Examples
```bash
./atom_example
```

## Usage Example

```cpp
#include "Atom.h"
#include "AtomBuilder.h"

using namespace BioMesh;

// Create atoms with different constructors
Atom hydrogen("H");                      // Element only
Atom carbon("C", 1.70);                  // Element + radius
Atom nitrogen("N", 1.55, 14.007);       // Element + radius + mass

// Use AtomBuilder for automatic property assignment
std::vector<Atom> parsedAtoms;
parsedAtoms.emplace_back(1.0, 2.0, 3.0, "C");  // From PDB parsing
parsedAtoms.emplace_back(4.0, 5.0, 6.0, "N");

AtomBuilder builder;
auto enhancedAtoms = builder.buildAtoms(parsedAtoms);  // Automatic radius/mass assignment

// Access properties
std::cout << "Carbon radius: " << enhancedAtoms[0].getAtomicRadius() << " Å\n";
std::cout << "Carbon mass: " << enhancedAtoms[0].getAtomicMass() << " Da\n";
```

## API Documentation

### Atom Class Methods

**Getters:**
- `getCoordinates()` - Returns 3D coordinates array
- `getX()`, `getY()`, `getZ()` - Individual coordinate access
- `getChemicalElement()` - Chemical element symbol
- `getAtomicRadius()` - Atomic radius in Angstroms
- `getAtomicMass()` - Atomic mass in Daltons

**Setters:**
- `setCoordinates(x, y, z)` - Set 3D coordinates
- `setX(x)`, `setY(y)`, `setZ(z)` - Set individual coordinates
- `setChemicalElement(element)` - Set chemical element
- `setAtomicRadius(radius)` - Set atomic radius
- `setAtomicMass(mass)` - Set atomic mass

### AtomBuilder Class Methods

- `buildAtoms(parsedAtoms)` - Build enhanced atoms with properties
- `addAtomicSpec(element, radius, mass)` - Add custom element specification
- `hasElement(element)` - Check if element exists in database
- `getAtomicSpec(element)` - Get atomic specification for element

## Testing

The project includes comprehensive unit tests covering:
- All Atom constructors and methods
- AtomBuilder functionality
- Error handling scenarios
- Integration tests with multiple elements

## License

This project is licensed under CC0 1.0 Universal - see the LICENSE file for details.
