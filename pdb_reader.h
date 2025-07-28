#ifndef PDB_READER_H
#define PDB_READER_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

// Structure to represent an atom in PDB format
struct Atom {
    int serial;           // Atom serial number
    std::string name;     // Atom name
    char altLoc;          // Alternate location indicator
    std::string resName;  // Residue name
    char chainID;         // Chain identifier
    int resSeq;           // Residue sequence number
    char iCode;           // Code for insertion of residues
    double x, y, z;       // Orthogonal coordinates for X, Y, Z in Angstroms
    double occupancy;     // Occupancy
    double tempFactor;    // Temperature factor
    std::string element;  // Element symbol
    std::string charge;   // Charge on the atom
};

// Structure to represent a PDB file
struct PDBData {
    std::string title;
    std::string date;
    std::string pdbID;
    std::vector<Atom> atoms;
    
    // Methods
    void clear();
    void printSummary() const;
    void printAtoms() const;
};

// Function declarations
bool readPDBFile(const std::string& filename, PDBData& pdbData);
Atom parseATOMLine(const std::string& line);
void printUsage(const char* programName);

#endif // PDB_READER_H