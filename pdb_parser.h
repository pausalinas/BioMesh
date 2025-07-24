#ifndef PDB_PARSER_H
#define PDB_PARSER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <array>

// Structure to represent an atom in 3D space
struct Atom {
    int serial;                 // Atom serial number
    std::string name;           // Atom name (e.g., "CA", "CB", "N")
    std::string altLoc;         // Alternate location indicator
    std::string resName;        // Residue name (e.g., "ALA", "GLY")
    char chainID;               // Chain identifier
    int resSeq;                 // Residue sequence number
    char iCode;                 // Code for insertion of residues
    double x, y, z;             // Atomic coordinates in Angstroms
    double occupancy;           // Occupancy value
    double tempFactor;          // Temperature factor (B-factor)
    std::string element;        // Element symbol
    std::string charge;         // Charge on the atom
    
    // Constructor
    Atom() : serial(0), chainID(' '), resSeq(0), iCode(' '), 
             x(0.0), y(0.0), z(0.0), occupancy(1.0), tempFactor(0.0) {}
    
    // Print atom information
    void print() const;
};

// Structure to represent the entire molecular structure
struct MolecularStructure {
    std::vector<Atom> atoms;
    std::string title;
    std::string header;
    
    // Add an atom to the structure
    void addAtom(const Atom& atom);
    
    // Get atoms from a specific chain
    std::vector<Atom> getChainAtoms(char chainID) const;
    
    // Get bounding box of the structure
    void getBoundingBox(double& minX, double& maxX, 
                       double& minY, double& maxY, 
                       double& minZ, double& maxZ) const;
    
    // Print structure summary
    void printSummary() const;
};

// Main PDB parser class
class PDBParser {
private:
    // Helper function to trim whitespace
    std::string trim(const std::string& str);
    
    // Helper function to parse double from string with error handling
    double parseDouble(const std::string& str, double defaultValue = 0.0);
    
    // Helper function to parse integer from string with error handling
    int parseInt(const std::string& str, int defaultValue = 0);
    
    // Parse ATOM/HETATM record
    bool parseAtomRecord(const std::string& line, Atom& atom);
    
public:
    // Main function to parse PDB file
    bool parsePDBFile(const std::string& filename, MolecularStructure& structure);
    
    // Function to validate PDB file format
    bool validatePDBFile(const std::string& filename);
    
    // Function to extract only coordinates (for mesh generation)
    std::vector<std::array<double, 3>> extractCoordinates(const MolecularStructure& structure);
};

#endif // PDB_PARSER_H