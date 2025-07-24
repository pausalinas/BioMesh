#include "pdb_parser.h"
#include <array>
#include <map>

// Atom methods
void Atom::print() const {
    std::cout << "ATOM " << std::setw(5) << serial 
              << " " << std::setw(4) << name 
              << " " << resName 
              << " " << chainID 
              << std::setw(4) << resSeq 
              << "    " << std::fixed << std::setprecision(3)
              << std::setw(8) << x 
              << std::setw(8) << y 
              << std::setw(8) << z 
              << " [" << element << "]" << std::endl;
}

// MolecularStructure methods
void MolecularStructure::addAtom(const Atom& atom) {
    atoms.push_back(atom);
}

std::vector<Atom> MolecularStructure::getChainAtoms(char chainID) const {
    std::vector<Atom> chainAtoms;
    for (const auto& atom : atoms) {
        if (atom.chainID == chainID) {
            chainAtoms.push_back(atom);
        }
    }
    return chainAtoms;
}

void MolecularStructure::getBoundingBox(double& minX, double& maxX, 
                                       double& minY, double& maxY, 
                                       double& minZ, double& maxZ) const {
    if (atoms.empty()) {
        minX = maxX = minY = maxY = minZ = maxZ = 0.0;
        return;
    }
    
    minX = maxX = atoms[0].x;
    minY = maxY = atoms[0].y;
    minZ = maxZ = atoms[0].z;
    
    for (const auto& atom : atoms) {
        minX = std::min(minX, atom.x);
        maxX = std::max(maxX, atom.x);
        minY = std::min(minY, atom.y);
        maxY = std::max(maxY, atom.y);
        minZ = std::min(minZ, atom.z);
        maxZ = std::max(maxZ, atom.z);
    }
}

void MolecularStructure::printSummary() const {
    std::cout << "\n=== Molecular Structure Summary ===" << std::endl;
    std::cout << "Title: " << title << std::endl;
    std::cout << "Total atoms: " << atoms.size() << std::endl;
    
    if (!atoms.empty()) {
        double minX, maxX, minY, maxY, minZ, maxZ;
        getBoundingBox(minX, maxX, minY, maxY, minZ, maxZ);
        
        std::cout << "Bounding box:" << std::endl;
        std::cout << "  X: [" << std::fixed << std::setprecision(3) 
                  << minX << ", " << maxX << "] (size: " << (maxX - minX) << ")" << std::endl;
        std::cout << "  Y: [" << minY << ", " << maxY << "] (size: " << (maxY - minY) << ")" << std::endl;
        std::cout << "  Z: [" << minZ << ", " << maxZ << "] (size: " << (maxZ - minZ) << ")" << std::endl;
    }
    
    // Count atoms per chain
    std::map<char, int> chainCounts;
    for (const auto& atom : atoms) {
        chainCounts[atom.chainID]++;
    }
    
    std::cout << "Atoms per chain:" << std::endl;
    for (const auto& pair : chainCounts) {
        std::cout << "  Chain " << (pair.first == ' ' ? '\'' : pair.first) 
                  << ": " << pair.second << " atoms" << std::endl;
    }
    std::cout << "===================================" << std::endl;
}

// PDBParser methods
std::string PDBParser::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

double PDBParser::parseDouble(const std::string& str, double defaultValue) {
    try {
        std::string trimmed = trim(str);
        if (trimmed.empty()) return defaultValue;
        return std::stod(trimmed);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

int PDBParser::parseInt(const std::string& str, int defaultValue) {
    try {
        std::string trimmed = trim(str);
        if (trimmed.empty()) return defaultValue;
        return std::stoi(trimmed);
    } catch (const std::exception&) {
        return defaultValue;
    }
}

bool PDBParser::parseAtomRecord(const std::string& line, Atom& atom) {
    // Check if line is long enough for basic ATOM/HETATM record
    if (line.length() < 54) {
        std::cerr << "Warning: Line too short for ATOM record: " << line << std::endl;
        return false;
    }
    
    try {
        // Parse according to PDB format specification
        // Columns 1-6: Record name (ATOM or HETATM)
        std::string recordName = line.substr(0, 6);
        recordName = trim(recordName);
        
        if (recordName != "ATOM" && recordName != "HETATM") {
            return false;
        }
        
        // Columns 7-11: Atom serial number
        atom.serial = parseInt(line.substr(6, 5), 0);
        
        // Columns 13-16: Atom name
        atom.name = trim(line.substr(12, 4));
        
        // Column 17: Alternate location indicator
        if (line.length() > 16) {
            atom.altLoc = line[16] != ' ' ? std::string(1, line[16]) : "";
        }
        
        // Columns 18-20: Residue name
        if (line.length() > 19) {
            atom.resName = trim(line.substr(17, 3));
        }
        
        // Column 22: Chain identifier
        if (line.length() > 21) {
            atom.chainID = line[21];
        }
        
        // Columns 23-26: Residue sequence number
        if (line.length() > 25) {
            atom.resSeq = parseInt(line.substr(22, 4), 0);
        }
        
        // Column 27: Code for insertion of residues
        if (line.length() > 26) {
            atom.iCode = line[26];
        }
        
        // Columns 31-38: X coordinate
        atom.x = parseDouble(line.substr(30, 8), 0.0);
        
        // Columns 39-46: Y coordinate
        atom.y = parseDouble(line.substr(38, 8), 0.0);
        
        // Columns 47-54: Z coordinate
        atom.z = parseDouble(line.substr(46, 8), 0.0);
        
        // Optional fields (may not be present in all PDB files)
        
        // Columns 55-60: Occupancy
        if (line.length() > 59) {
            atom.occupancy = parseDouble(line.substr(54, 6), 1.0);
        }
        
        // Columns 61-66: Temperature factor
        if (line.length() > 65) {
            atom.tempFactor = parseDouble(line.substr(60, 6), 0.0);
        }
        
        // Columns 77-78: Element symbol
        if (line.length() > 77) {
            atom.element = trim(line.substr(76, 2));
        }
        
        // Columns 79-80: Charge
        if (line.length() > 79) {
            atom.charge = trim(line.substr(78, 2));
        }
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error parsing atom record: " << e.what() << std::endl;
        std::cerr << "Line: " << line << std::endl;
        return false;
    }
}

bool PDBParser::validatePDBFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    bool hasAtomRecords = false;
    int lineCount = 0;
    
    while (std::getline(file, line) && lineCount < 100) { // Check first 100 lines
        lineCount++;
        
        if (line.length() >= 6) {
            std::string recordType = trim(line.substr(0, 6));
            if (recordType == "ATOM" || recordType == "HETATM") {
                hasAtomRecords = true;
                break;
            }
        }
    }
    
    file.close();
    
    if (!hasAtomRecords) {
        std::cerr << "Warning: No ATOM or HETATM records found in the first " 
                  << lineCount << " lines" << std::endl;
    }
    
    return hasAtomRecords;
}

bool PDBParser::parsePDBFile(const std::string& filename, MolecularStructure& structure) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file " << filename << std::endl;
        return false;
    }
    
    std::cout << "Parsing PDB file: " << filename << std::endl;
    
    std::string line;
    int atomCount = 0;
    int lineNumber = 0;
    
    // Clear any existing data
    structure.atoms.clear();
    structure.title.clear();
    structure.header.clear();
    
    while (std::getline(file, line)) {
        lineNumber++;
        
        if (line.empty()) continue;
        
        // Ensure line has at least 6 characters for record type
        if (line.length() < 6) continue;
        
        std::string recordType = trim(line.substr(0, 6));
        
        // Handle different record types
        if (recordType == "HEADER") {
            structure.header = trim(line.substr(6));
        }
        else if (recordType == "TITLE") {
            if (!structure.title.empty()) structure.title += " ";
            structure.title += trim(line.substr(6));
        }
        else if (recordType == "MODEL") {
            // For simplicity, we'll only process the first model
            if (atomCount > 0) {
                std::cout << "Multiple models detected. Processing only the first model." << std::endl;
                break;
            }
        }
        else if (recordType == "ENDMDL") {
            // Stop after first model
            break;
        }
        else if (recordType == "ATOM" || recordType == "HETATM") {
            Atom atom;
            if (parseAtomRecord(line, atom)) {
                structure.addAtom(atom);
                atomCount++;
            } else {
                std::cerr << "Warning: Failed to parse line " << lineNumber 
                          << ": " << line << std::endl;
            }
        }
        else if (recordType == "END") {
            break;
        }
        // Ignore other record types (REMARK, CONECT, etc.)
    }
    
    file.close();
    
    std::cout << "Successfully parsed " << atomCount << " atoms from " 
              << lineNumber << " lines" << std::endl;
    
    return atomCount > 0;
}

std::vector<std::array<double, 3>> PDBParser::extractCoordinates(const MolecularStructure& structure) {
    std::vector<std::array<double, 3>> coordinates;
    coordinates.reserve(structure.atoms.size());
    
    for (const auto& atom : structure.atoms) {
        coordinates.push_back({atom.x, atom.y, atom.z});
    }
    
    return coordinates;
}