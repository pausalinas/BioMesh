#include "PDBReader.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>

namespace BioMesh {

PDBReader::PDBReader() {}

PDBReader::~PDBReader() {}

bool PDBReader::readFile(const std::string& filename) {
    atoms_.clear();
    warnings_.clear();
    errors_.clear();
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        addError("Cannot open file: " + filename);
        return false;
    }
    
    std::string line;
    int lineNumber = 0;
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        if (line.empty()) continue;
        
        // Check for HETATM records and warn about limitation
        if (line.substr(0, 6) == "HETATM") {
            addWarning("Line " + std::to_string(lineNumber) + 
                      ": HETATM record encountered but not parsed (current limitation)");
            continue;
        }
        
        // Parse ATOM records only
        if (line.substr(0, 4) == "ATOM") {
            if (!isValidAtomLine(line)) {
                addError("Line " + std::to_string(lineNumber) + 
                        ": Malformed ATOM record");
                continue;
            }
            
            Atom atom;
            if (parseAtomLine(line, atom)) {
                atoms_.push_back(atom);
            } else {
                addError("Line " + std::to_string(lineNumber) + 
                        ": Failed to parse ATOM record");
            }
        }
    }
    
    file.close();
    
    if (atoms_.empty() && errors_.empty()) {
        addWarning("No ATOM records found in file");
    }
    
    // Return false only for critical errors (file not found, etc.)
    // Return true even if there were parsing errors, as long as we got some atoms
    // or there were no critical errors
    return !atoms_.empty() || errors_.empty();
}

bool PDBReader::parseAtomLine(const std::string& line, Atom& atom) {
    if (line.length() < 54) {
        return false;
    }
    
    try {
        // Parse according to PDB format specification
        atom.serial = std::stoi(line.substr(6, 5));
        atom.name = line.substr(12, 4);
        // Remove leading/trailing spaces
        atom.name.erase(0, atom.name.find_first_not_of(" "));
        atom.name.erase(atom.name.find_last_not_of(" ") + 1);
        
        atom.resName = line.substr(17, 3);
        atom.resName.erase(0, atom.resName.find_first_not_of(" "));
        atom.resName.erase(atom.resName.find_last_not_of(" ") + 1);
        
        atom.chainID = line.length() > 21 ? line[21] : ' ';
        atom.resSeq = std::stoi(line.substr(22, 4));
        
        atom.x = std::stod(line.substr(30, 8));
        atom.y = std::stod(line.substr(38, 8));
        atom.z = std::stod(line.substr(46, 8));
        
        // Optional fields
        if (line.length() > 54) {
            atom.occupancy = std::stod(line.substr(54, 6));
        }
        if (line.length() > 60) {
            atom.tempFactor = std::stod(line.substr(60, 6));
        }
        if (line.length() > 76) {
            atom.element = line.substr(76, 2);
            atom.element.erase(0, atom.element.find_first_not_of(" "));
            atom.element.erase(atom.element.find_last_not_of(" ") + 1);
        }
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

bool PDBReader::isValidAtomLine(const std::string& line) const {
    // Basic validation for ATOM line
    return line.length() >= 54 && 
           line.substr(0, 4) == "ATOM";
}

void PDBReader::addWarning(const std::string& warning) {
    warnings_.push_back(warning);
}

void PDBReader::addError(const std::string& error) {
    errors_.push_back(error);
}

void PDBReader::printStatistics() const {
    std::cout << "=== PDB File Statistics ===" << std::endl;
    std::cout << "Total ATOM records: " << atoms_.size() << std::endl;
    
    // Count by chain
    std::map<char, int> chainCounts;
    for (const auto& atom : atoms_) {
        chainCounts[atom.chainID]++;
    }
    
    std::cout << "Atoms by chain:" << std::endl;
    for (const auto& pair : chainCounts) {
        char chain = pair.first == ' ' ? '_' : pair.first;
        std::cout << "  Chain " << chain << ": " << pair.second << " atoms" << std::endl;
    }
    
    std::cout << "Coordinate ranges:" << std::endl;
    std::cout << getCoordinateRanges() << std::endl;
    
    if (!warnings_.empty()) {
        std::cout << "Warnings (" << warnings_.size() << "):" << std::endl;
        for (const auto& warning : warnings_) {
            std::cout << "  " << warning << std::endl;
        }
    }
    
    if (!errors_.empty()) {
        std::cout << "Errors (" << errors_.size() << "):" << std::endl;
        for (const auto& error : errors_) {
            std::cout << "  " << error << std::endl;
        }
    }
}

std::string PDBReader::getCoordinateRanges() const {
    if (atoms_.empty()) {
        return "No atoms to analyze";
    }
    
    double minX = atoms_[0].x, maxX = atoms_[0].x;
    double minY = atoms_[0].y, maxY = atoms_[0].y;
    double minZ = atoms_[0].z, maxZ = atoms_[0].z;
    
    for (const auto& atom : atoms_) {
        minX = std::min(minX, atom.x);
        maxX = std::max(maxX, atom.x);
        minY = std::min(minY, atom.y);
        maxY = std::max(maxY, atom.y);
        minZ = std::min(minZ, atom.z);
        maxZ = std::max(maxZ, atom.z);
    }
    
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(3);
    oss << "  X: " << minX << " to " << maxX << " (range: " << (maxX - minX) << ")" << std::endl;
    oss << "  Y: " << minY << " to " << maxY << " (range: " << (maxY - minY) << ")" << std::endl;
    oss << "  Z: " << minZ << " to " << maxZ << " (range: " << (maxZ - minZ) << ")";
    
    return oss.str();
}

int PDBReader::getAtomCountByChain(char chainID) const {
    return std::count_if(atoms_.begin(), atoms_.end(),
                        [chainID](const Atom& atom) {
                            return atom.chainID == chainID;
                        });
}

} // namespace BioMesh