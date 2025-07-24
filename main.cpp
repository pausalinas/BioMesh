#include "pdb_parser.h"
#include <iostream>
#include <string>
#include <algorithm> // For std::min and std::max
#include <iomanip> // For std::fixed and std::setprecision

void printUsage(const std::string& programName) {
    std::cout << "Usage: " << programName << " <pdb_file>" << std::endl;
    std::cout << "       " << programName << " --help" << std::endl;
    std::cout << std::endl;
    std::cout << "BioMesh PDB Parser - Extracts atomic coordinates from PDB files" << std::endl;
    std::cout << "for volumetric mesh generation using octree algorithms." << std::endl;
    std::cout << std::endl;
    std::cout << "Arguments:" << std::endl;
    std::cout << "  pdb_file    Path to the PDB file to parse" << std::endl;
    std::cout << "  --help      Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Example:" << std::endl;
    std::cout << "  " << programName << " protein.pdb" << std::endl;
}

void demonstrateCoordinateExtraction(const MolecularStructure& structure) {
    PDBParser parser;
    auto coordinates = parser.extractCoordinates(structure);
    
    std::cout << "\n=== Coordinate Extraction for Mesh Generation ===" << std::endl;
    std::cout << "Extracted " << coordinates.size() << " coordinate points:" << std::endl;
    
    // Show first 10 coordinates as example
    int showCount = std::min(10, static_cast<int>(coordinates.size()));
    for (int i = 0; i < showCount; i++) {
        std::cout << "  Point " << i + 1 << ": ("
                  << std::fixed << std::setprecision(3)
                  << coordinates[i][0] << ", "
                  << coordinates[i][1] << ", "
                  << coordinates[i][2] << ")" << std::endl;
    }
    
    if (coordinates.size() > 10) {
        std::cout << "  ... and " << (coordinates.size() - 10) << " more points" << std::endl;
    }
    
    std::cout << "=================================================" << std::endl;
}

void analyzeStructure(const MolecularStructure& structure) {
    std::cout << "\n=== Structure Analysis for BioMesh ===" << std::endl;
    
    // Get bounding box for octree initialization
    double minX, maxX, minY, maxY, minZ, maxZ;
    structure.getBoundingBox(minX, maxX, minY, maxY, minZ, maxZ);
    
    // Calculate center and dimensions
    double centerX = (minX + maxX) / 2.0;
    double centerY = (minY + maxY) / 2.0;
    double centerZ = (minZ + maxZ) / 2.0;
    
    double sizeX = maxX - minX;
    double sizeY = maxY - minY;
    double sizeZ = maxZ - minZ;
    
    double maxDim = std::max({sizeX, sizeY, sizeZ});
    
    std::cout << "Structure center: (" << std::fixed << std::setprecision(3)
              << centerX << ", " << centerY << ", " << centerZ << ")" << std::endl;
    std::cout << "Dimensions: " << sizeX << " x " << sizeY << " x " << sizeZ << " Å" << std::endl;
    std::cout << "Maximum dimension: " << maxDim << " Å" << std::endl;
    std::cout << "Suggested octree root size: " << (maxDim * 1.2) << " Å" << std::endl;
    
    // Atom density analysis
    double volume = sizeX * sizeY * sizeZ;
    double density = structure.atoms.size() / volume;
    std::cout << "Atom density: " << std::scientific << std::setprecision(2)
              << density << " atoms/Å³" << std::endl;
    
    std::cout << "======================================" << std::endl;
}

int main(int argc, char* argv[]) {
    // Handle command line arguments
    if (argc < 2) {
        std::cerr << "Error: No PDB file specified." << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    std::string argument(argv[1]);
    if (argument == "--help" || argument == "-h") {
        printUsage(argv[0]);
        return 0;
    }
    
    std::string pdbFilename = argument;
    
    // Create parser and structure objects
    PDBParser parser;
    MolecularStructure structure;
    
    std::cout << "BioMesh PDB Parser v1.0" << std::endl;
    std::cout << "=======================" << std::endl;
    
    // Validate the PDB file first
    std::cout << "Validating PDB file format..." << std::endl;
    if (!parser.validatePDBFile(pdbFilename)) {
        std::cerr << "Error: Invalid or unreadable PDB file: " << pdbFilename << std::endl;
        return 1;
    }
    std::cout << "PDB file format validation passed." << std::endl;
    
    // Parse the PDB file
    std::cout << "\nParsing PDB file..." << std::endl;
    if (!parser.parsePDBFile(pdbFilename, structure)) {
        std::cerr << "Error: Failed to parse PDB file: " << pdbFilename << std::endl;
        return 1;
    }
    
    // Display structure summary
    structure.printSummary();
    
    // Demonstrate coordinate extraction for mesh generation
    demonstrateCoordinateExtraction(structure);
    
    // Analyze structure for BioMesh processing
    analyzeStructure(structure);
    
    // Show first few atoms in detail
    std::cout << "\n=== Sample Atom Records ===" << std::endl;
    int sampleCount = std::min(5, static_cast<int>(structure.atoms.size()));
    for (int i = 0; i < sampleCount; i++) {
        structure.atoms[i].print();
    }
    if (structure.atoms.size() > 5) {
        std::cout << "... and " << (structure.atoms.size() - 5) << " more atoms" << std::endl;
    }
    std::cout << "===========================" << std::endl;
    
    std::cout << "\nPDB parsing completed successfully!" << std::endl;
    std::cout << "Ready for octree-based mesh generation." << std::endl;
    
    return 0;
}