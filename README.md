# PDB Reader Library

A modern C++ library for reading and parsing PDB (Protein Data Bank) structure files. This library provides a clean, robust interface for extracting atomic information from PDB files with comprehensive error handling and modern C++ best practices.

## Features

- **Modern C++17**: Uses smart pointers, RAII, and modern STL containers
- **Robust Error Handling**: Custom exceptions for different error types
- **Comprehensive Testing**: Unit tests using Google Test framework
- **Thread-Safe**: Stateless design allows concurrent usage
- **Memory Efficient**: Uses smart pointers and move semantics
- **Cross-Platform**: CMake build system with Linux, Windows, and macOS support

## Quick Start

### Building the Library

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
```

### Running Tests

```bash
# Build and run all tests
make run_tests

# Or run tests with CTest
ctest --verbose
```

### Basic Usage

```cpp
#include "PDBReader.h"
#include <iostream>

int main() {
    try {
        pdb::PDBReader reader;
        auto atoms = reader.readFile("protein.pdb");
        
        std::cout << "Parsed " << reader.getAtomCount() << " atoms\n";
        
        // Access atom properties
        for (const auto& atom : atoms) {
            std::cout << "Atom " << atom->getId() 
                      << ": " << atom->getType()
                      << " at (" << atom->getX() 
                      << ", " << atom->getY() 
                      << ", " << atom->getZ() << ")\n";
        }
    } catch (const pdb::PDBFileException& e) {
        std::cerr << "File error: " << e.what() << std::endl;
    } catch (const pdb::PDBParsingException& e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
    }
    return 0;
}
```

## API Reference

### Classes

#### `pdb::Atom`
Represents an atom with its type and 3D coordinates.

**Constructor:**
```cpp
Atom(int id, std::string type, double x, double y, double z)
```

**Methods:**
- `int getId() const` - Returns the sequential atom identifier
- `const std::string& getType() const` - Returns the atom type (e.g., "CA", "N", "O")
- `double getX() const` - Returns X coordinate
- `double getY() const` - Returns Y coordinate  
- `double getZ() const` - Returns Z coordinate
- `double distanceFromOrigin() const` - Calculates distance from origin
- `double distanceTo(const Atom& other) const` - Calculates distance to another atom

#### `pdb::PDBReader`
Main class for reading and parsing PDB files.

**Methods:**
- `AtomContainer readFile(const std::string& filename)` - Reads atoms from a PDB file
- `AtomContainer parseContent(const std::string& content)` - Parses atoms from string content
- `size_t getAtomCount() const` - Returns number of atoms from last operation
- `size_t getLinesProcessed() const` - Returns number of lines processed
- `static bool isValidAtomLine(const std::string& line)` - Validates if line is an ATOM record

**Type Aliases:**
```cpp
using AtomContainer = std::vector<std::unique_ptr<Atom>>;
```

### Exceptions

#### `pdb::PDBFileException`
Thrown for file I/O errors (file not found, permission denied, etc.)

#### `pdb::PDBParsingException`  
Thrown for parsing errors (malformed lines, invalid data, etc.)

## PDB Format Support

The library processes lines that start with "ATOM" and extracts:
- **Columns 13-16**: Atom name/type
- **Columns 31-38**: X coordinate (Angstroms)
- **Columns 39-46**: Y coordinate (Angstroms)  
- **Columns 47-54**: Z coordinate (Angstroms)

Non-ATOM lines (HEADER, REMARK, HETATM, etc.) are silently skipped.

## Build Options

### CMake Options

- `BUILD_TESTS` (default: ON) - Build unit tests
- `BUILD_EXAMPLES` (default: ON) - Build example programs
- `CMAKE_BUILD_TYPE` - Build type (Debug/Release)

### Compiler Requirements

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2019+)
- CMake 3.16 or later

### Dependencies

- **Google Test** (automatically downloaded if not found)
- **Standard C++ Library** (no external dependencies for core library)

## Examples

The `examples/` directory contains:

1. **basic_usage.cpp** - Simple file reading and atom iteration
2. **advanced_usage.cpp** - Statistical analysis, distance calculations, spatial searches

Run examples:
```bash
./build/examples/basic_usage
./build/examples/advanced_usage
```

## Testing

The test suite includes:

- **Unit Tests**: Comprehensive coverage of Atom and PDBReader classes
- **Error Handling Tests**: Validation of exception handling
- **Edge Case Tests**: Empty files, malformed data, boundary conditions
- **Integration Tests**: End-to-end file reading scenarios

Test coverage includes:
- Valid and invalid PDB formats
- File I/O error conditions
- Coordinate parsing (positive, negative, scientific notation)
- Memory management and move semantics
- Thread safety considerations

## Design Principles

### RAII (Resource Acquisition Is Initialization)
- File handles automatically closed
- Memory automatically managed via smart pointers
- Exception-safe resource management

### Single Responsibility Principle
- `Atom` class: Represents atomic data
- `PDBReader` class: Handles file parsing
- Separate exception types for different error conditions

### Modern C++ Features
- Smart pointers (`std::unique_ptr`) instead of raw pointers
- Move semantics for efficient data transfer
- `constexpr` and `noexcept` where appropriate
- Range-based for loops and STL algorithms

### Error Handling Strategy
- Custom exception hierarchy for different error types
- Detailed error messages with line numbers for parsing errors
- Graceful handling of malformed input data
- No silent failures - all errors are reported

## Performance Considerations

- **Memory Efficient**: Uses move semantics to avoid unnecessary copies
- **Parsing Optimized**: Single-pass parsing with minimal string operations
- **Container Optimized**: `std::vector` with smart pointers for cache efficiency
- **Thread Safe**: Stateless design allows concurrent usage

## Contributing

1. Follow the existing code style and conventions
2. Add unit tests for new features
3. Update documentation for API changes
4. Ensure all tests pass before submitting

## License

This project is provided as-is for educational and research purposes.

## Acknowledgments

- PDB format specification from the Protein Data Bank
- Google Test framework for unit testing
- Modern C++ design patterns and best practices
