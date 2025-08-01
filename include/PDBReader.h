#pragma once

#include <string>
#include <vector>
#include <fstream>

namespace BioMesh {

struct Atom {
    int serial;
    std::string name;
    std::string resName;
    char chainID;
    int resSeq;
    double x, y, z;
    double occupancy;
    double tempFactor;
    std::string element;
    
    Atom() : serial(0), chainID(' '), resSeq(0), x(0.0), y(0.0), z(0.0), 
             occupancy(1.0), tempFactor(0.0) {}
};

class PDBReader {
public:
    PDBReader();
    ~PDBReader();
    
    // Main parsing function
    bool readFile(const std::string& filename);
    
    // Accessors
    const std::vector<Atom>& getAtoms() const { return atoms_; }
    size_t getAtomCount() const { return atoms_.size(); }
    
    // Statistics
    void printStatistics() const;
    std::string getCoordinateRanges() const;
    int getAtomCountByChain(char chainID) const;
    
    // Error handling
    const std::vector<std::string>& getWarnings() const { return warnings_; }
    const std::vector<std::string>& getErrors() const { return errors_; }
    bool hasErrors() const { return !errors_.empty(); }
    
private:
    std::vector<Atom> atoms_;
    std::vector<std::string> warnings_;
    std::vector<std::string> errors_;
    
    // Helper methods
    bool parseAtomLine(const std::string& line, Atom& atom);
    void addWarning(const std::string& warning);
    void addError(const std::string& error);
    bool isValidAtomLine(const std::string& line) const;
};

} // namespace BioMesh