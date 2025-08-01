# BioMesh

BioMesh is a software tool designed to generate volumetric meshes built with hexahedrons (cubes) by recursive subdivision of space based on an octree algorithm. It includes comprehensive PDB file reading capabilities for processing molecular structures.

## Features

### PDB File Processing
- **ATOM Record Support**: Full parsing of ATOM records from PDB files
- **Coordinate Extraction**: Precise parsing of atomic coordinates (X, Y, Z)
- **Molecular Properties**: Reads atom names, residue information, chain IDs, occupancy, and temperature factors
- **Multi-chain Support**: Handles proteins with multiple chains
- **Error Handling**: Robust error detection and reporting for malformed files
- **Performance Optimized**: Efficient parsing of large PDB files (tested with 10,000+ atoms)

### Current Limitations
- **HETATM Records**: Currently not parsed (generates warnings)
- **Supported Record Types**: Only ATOM records are processed
- **File Format**: Standard PDB format only

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Usage

### PDB Viewer Application
The `pdb_viewer` application provides command-line access to PDB parsing capabilities:

```bash
# Basic usage
./pdb_viewer protein.pdb

# Verbose output with first 10 atoms
./pdb_viewer --verbose protein.pdb

# Show detailed statistics
./pdb_viewer --stats protein.pdb

# Combined options
./pdb_viewer --verbose --stats protein.pdb
```

### Example Output
```
Successfully parsed PDB file: protein.pdb
Found 1432 ATOM records
Warnings (3):
  Line 145: HETATM record encountered but not parsed (current limitation)
  Line 146: HETATM record encountered but not parsed (current limitation)
  Line 147: HETATM record encountered but not parsed (current limitation)

=== PDB File Statistics ===
Total ATOM records: 1432
Atoms by chain:
  Chain A: 716 atoms
  Chain B: 716 atoms
Coordinate ranges:
  X: -25.123 to 34.567 (range: 59.690)
  Y: -18.456 to 42.789 (range: 61.245)
  Z: -12.345 to 38.901 (range: 51.246)
```

### Programming Interface

```cpp
#include "PDBReader.h"

BioMesh::PDBReader reader;
if (reader.readFile("protein.pdb")) {
    std::cout << "Found " << reader.getAtomCount() << " atoms\n";
    
    // Access atoms
    const auto& atoms = reader.getAtoms();
    for (const auto& atom : atoms) {
        std::cout << atom.name << " at (" 
                  << atom.x << ", " << atom.y << ", " << atom.z << ")\n";
    }
    
    // Check for warnings/errors
    if (!reader.getWarnings().empty()) {
        std::cout << "Warnings:\n";
        for (const auto& warning : reader.getWarnings()) {
            std::cout << "  " << warning << "\n";
        }
    }
}
```

## Testing

### Test Suite
Comprehensive testing is provided through unit and integration tests:

```bash
# Run unit tests
./unit_tests

# Run integration tests (including performance tests)
./integration_tests

# Run all tests
make test
```

### Test Data Generation
Generate synthetic test data for various scenarios:

```bash
# Generate large protein (1000+ residues)
./test_generator --large large_test.pdb 1000

# Generate edge case test files
./test_generator --edge-cases test_directory/

# Generate all test files
./test_generator --all test_directory/
```

### Available Test Files
The test suite includes various PDB file types in `tests/test_data/`:

- **protein_small.pdb**: Small protein structure (16 atoms)
- **ligand_hetatm.pdb**: Small molecule with HETATM records (demonstrates limitation)
- **mixed_structure.pdb**: Protein with ligand (ATOM + HETATM records)
- **malformed.pdb**: File with parsing errors for error handling testing
- **large_protein.pdb**: Large structure for performance testing (4000+ atoms)
- **empty.pdb**: Empty file for edge case testing
- **extreme_coords.pdb**: Atoms at coordinate extremes
- **missing_coords.pdb**: Malformed coordinate fields

### Test Coverage
- ✅ Basic PDB file reading
- ✅ Error handling for non-existent files
- ✅ Empty file handling
- ✅ Malformed ATOM line detection
- ✅ HETATM warning generation
- ✅ Coordinate parsing accuracy
- ✅ Atom property extraction
- ✅ Multi-chain structure support
- ✅ Performance with large files (5000+ atoms < 1 second)
- ✅ Memory handling for large structures
- ✅ Edge case file formats
- ✅ Error accumulation and reporting

## Supported PDB Format

### ATOM Records (Supported)
```
ATOM    123  CA  ALA A  45      10.000  20.000  30.000  1.00 15.00           C
```

### HETATM Records (Currently Not Supported)
```
HETATM  456  C1  LIG B 201     15.000  25.000  35.000  1.00 20.00           C
```
*Note: HETATM records generate warnings but are not parsed*

### Parsed Fields
- Serial number
- Atom name
- Residue name
- Chain ID
- Residue sequence number
- X, Y, Z coordinates
- Occupancy
- Temperature factor
- Element symbol

## Error Handling

The PDBReader provides comprehensive error reporting:

### Warning Types
- HETATM records encountered (current limitation)
- No ATOM records found in file
- Empty files

### Error Types
- File not found or cannot be opened
- Malformed ATOM record lines
- Invalid coordinate values
- Missing required fields

### Usage Example
```cpp
BioMesh::PDBReader reader;
if (!reader.readFile("problematic.pdb")) {
    // Handle critical errors
    for (const auto& error : reader.getErrors()) {
        std::cerr << "Error: " << error << std::endl;
    }
} else {
    // Process warnings
    for (const auto& warning : reader.getWarnings()) {
        std::cout << "Warning: " << warning << std::endl;
    }
}
```

## Performance

Performance benchmarks on test systems:
- **Small files** (< 100 atoms): < 1ms
- **Medium files** (1,000 atoms): < 10ms
- **Large files** (5,000 atoms): < 100ms
- **Very large files** (10,000+ atoms): < 1 second

Memory usage scales linearly with atom count (~80 bytes per atom).

## Future Enhancements

- [ ] HETATM record support
- [ ] Bond information parsing
- [ ] Secondary structure records
- [ ] Compressed PDB file support (.pdb.gz)
- [ ] mmCIF format support
- [ ] Biological assembly information

## Contributing

When contributing to the PDB parsing functionality:

1. Add tests for new features in `tests/unit_tests.cpp`
2. Include performance tests for large data in `tests/integration_tests.cpp`
3. Update test data in `tests/test_data/` as needed
4. Run full test suite before submitting changes
5. Document new limitations or supported features
