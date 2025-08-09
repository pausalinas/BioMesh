#include "PDBReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace BioMesh {

bool PDBReader::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }

    return parseContent(content);
}

bool PDBReader::parseContent(const std::string& pdbContent) {
    clear();
    
    std::istringstream stream(pdbContent);
    std::string line;
    
    while (std::getline(stream, line)) {
        // Only process ATOM records
        if (line.length() >= 6 && line.substr(0, 6) == "ATOM  ") {
            auto atom = parseAtomLine(line);
            if (atom) {
                atoms_.emplace_back(*atom);
            }
        }
    }
    
    return !atoms_.empty();
}

void PDBReader::clear() {
    atoms_.clear();
}

std::unique_ptr<Atom> PDBReader::parseAtomLine(const std::string& line) {
    // PDB format specification for ATOM records:
    // COLUMNS    DATA  TYPE      FIELD         DEFINITION
    // 1 -  6     Record name     "ATOM  "
    // 7 - 11     Integer         serial        Atom serial number
    // 13 - 16    Atom            name          Atom name
    // 17         Character       altLoc        Alternate location indicator
    // 18 - 20    Residue name    resName       Residue name
    // 22         Character       chainID       Chain identifier
    // 23 - 26    Integer         resSeq        Residue sequence number
    // 31 - 38    Real(8.3)       x             Orthogonal coordinates for X in Angstroms
    // 39 - 46    Real(8.3)       y             Orthogonal coordinates for Y in Angstroms
    // 47 - 54    Real(8.3)       z             Orthogonal coordinates for Z in Angstroms
    // 77 - 78    LString(2)      element       Element symbol, right-justified

    if (line.length() < 54) {
        return nullptr; // Line too short to contain coordinates
    }

    try {
        // Extract coordinates
        double x = std::stod(extractField(line, 30, 8));
        double y = std::stod(extractField(line, 38, 8));
        double z = std::stod(extractField(line, 46, 8));

        // Extract atom name
        std::string atomName = extractField(line, 12, 4);

        // Extract element symbol (if available)
        std::string element;
        if (line.length() >= 78) {
            element = extractField(line, 76, 2);
        }
        
        // If element is not specified, try to infer from atom name
        if (element.empty() && !atomName.empty()) {
            // Take the first alphabetic character(s) as element
            element = "";
            for (char c : atomName) {
                if (std::isalpha(c)) {
                    element += c;
                    if (element.length() >= 2) break;
                }
            }
        }

        return std::make_unique<Atom>(x, y, z, element, atomName);
    }
    catch (const std::exception&) {
        return nullptr; // Parsing failed
    }
}

std::string PDBReader::extractField(const std::string& line, size_t start, size_t length) {
    if (start >= line.length()) {
        return "";
    }
    
    size_t actualLength = std::min(length, line.length() - start);
    return trim(line.substr(start, actualLength));
}

std::string PDBReader::trim(const std::string& str) {
    auto start = str.begin();
    auto end = str.end();
    
    // Trim leading whitespace
    while (start != end && std::isspace(*start)) {
        ++start;
    }
    
    // Trim trailing whitespace
    while (start != end && std::isspace(*(end - 1))) {
        --end;
    }
    
    return std::string(start, end);
}

} // namespace BioMesh