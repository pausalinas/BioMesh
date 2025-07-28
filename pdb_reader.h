#ifndef PDB_READER_H
#define PDB_READER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

/**
 * Structure to represent an ATOM/HETATM record in a PDB file
 */
struct PDBAtom {
    std::string record_name;    // ATOM or HETATM
    int atom_number;           // Atom serial number
    std::string atom_name;     // Atom name
    char alt_loc;              // Alternate location indicator
    std::string residue_name;  // Residue name
    char chain_id;             // Chain identifier
    int residue_number;        // Residue sequence number
    char insertion_code;       // Code for insertion of residues
    double x, y, z;           // Orthogonal coordinates (Angstroms)
    double occupancy;         // Occupancy
    double temp_factor;       // Temperature factor
    std::string element;      // Element symbol
    std::string charge;       // Charge on the atom
};

/**
 * Class to handle PDB file reading and parsing
 */
class PDBReader {
private:
    std::string filename;
    std::vector<PDBAtom> atoms;
    std::string header;
    bool file_loaded;

public:
    PDBReader();
    PDBReader(const std::string& filename);
    ~PDBReader();
    
    // Core functionality
    bool openFile(const std::string& filename);
    bool parseFile();
    void printAtoms() const;
    void printSummary() const;
    
    // Getters
    const std::vector<PDBAtom>& getAtoms() const;
    const std::string& getHeader() const;
    int getAtomCount() const;
    bool isLoaded() const;
    
private:
    PDBAtom parseAtomLine(const std::string& line);
    std::string trim(const std::string& str);
};

#endif // PDB_READER_H