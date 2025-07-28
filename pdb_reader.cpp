#include "pdb_reader.h"
#include <algorithm>
#include <cctype>

// Helper function to trim whitespace
std::string trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

// Helper function to safely extract substring and convert to number
template<typename T>
T safeExtract(const std::string& line, size_t start, size_t length, T defaultValue) {
    if (start >= line.length()) return defaultValue;
    
    size_t actualLength = std::min(length, line.length() - start);
    std::string substr = trim(line.substr(start, actualLength));
    
    if (substr.empty()) return defaultValue;
    
    try {
        if constexpr (std::is_same_v<T, int>) {
            return std::stoi(substr);
        } else if constexpr (std::is_same_v<T, double>) {
            return std::stod(substr);
        }
    } catch (const std::exception&) {
        return defaultValue;
    }
    return defaultValue;
}

// Helper function to safely extract substring
std::string safeExtractString(const std::string& line, size_t start, size_t length) {
    if (start >= line.length()) return "";
    size_t actualLength = std::min(length, line.length() - start);
    return trim(line.substr(start, actualLength));
}

// Helper function to safely extract single character
char safeExtractChar(const std::string& line, size_t pos) {
    if (pos >= line.length()) return ' ';
    return line[pos];
}

// Parse an ATOM line from PDB format
Atom parseATOMLine(const std::string& line) {
    Atom atom;
    
    // PDB format specification for ATOM records
    atom.serial = safeExtract<int>(line, 6, 5, 0);
    atom.name = safeExtractString(line, 12, 4);
    atom.altLoc = safeExtractChar(line, 16);
    atom.resName = safeExtractString(line, 17, 3);
    atom.chainID = safeExtractChar(line, 21);
    atom.resSeq = safeExtract<int>(line, 22, 4, 0);
    atom.iCode = safeExtractChar(line, 26);
    atom.x = safeExtract<double>(line, 30, 8, 0.0);
    atom.y = safeExtract<double>(line, 38, 8, 0.0);
    atom.z = safeExtract<double>(line, 46, 8, 0.0);
    atom.occupancy = safeExtract<double>(line, 54, 6, 1.0);
    atom.tempFactor = safeExtract<double>(line, 60, 6, 0.0);
    atom.element = safeExtractString(line, 76, 2);
    atom.charge = safeExtractString(line, 78, 2);
    
    return atom;
}

// Read PDB file and populate PDBData structure
bool readPDBFile(const std::string& filename, PDBData& pdbData) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::cerr << "Error: Cannot open file '" << filename << "'" << std::endl;
        return false;
    }
    
    pdbData.clear();
    std::string line;
    
    while (std::getline(file, line)) {
        if (line.length() < 6) continue;
        
        std::string recordType = line.substr(0, 6);
        
        // Remove trailing whitespace from record type
        recordType.erase(std::find_if(recordType.rbegin(), recordType.rend(),
                        [](unsigned char ch) { return !std::isspace(ch); }).base(),
                        recordType.end());
        
        if (recordType == "HEADER") {
            if (line.length() > 50) {
                pdbData.pdbID = safeExtractString(line, 62, 4);
            }
            if (line.length() > 20) {
                pdbData.date = safeExtractString(line, 50, 9);
            }
        }
        else if (recordType == "TITLE") {
            std::string titlePart = safeExtractString(line, 10, 70);
            if (pdbData.title.empty()) {
                pdbData.title = titlePart;
            } else {
                pdbData.title += " " + titlePart;
            }
        }
        else if (recordType == "ATOM" || recordType == "HETATM") {
            Atom atom = parseATOMLine(line);
            pdbData.atoms.push_back(atom);
        }
    }
    
    file.close();
    
    std::cout << "Successfully read PDB file: " << filename << std::endl;
    std::cout << "Total atoms loaded: " << pdbData.atoms.size() << std::endl;
    
    return true;
}

// Clear PDBData structure
void PDBData::clear() {
    title.clear();
    date.clear();
    pdbID.clear();
    atoms.clear();
}

// Print summary information about the PDB file
void PDBData::printSummary() const {
    std::cout << "\n=== PDB File Summary ===" << std::endl;
    std::cout << "PDB ID: " << (pdbID.empty() ? "Not specified" : pdbID) << std::endl;
    std::cout << "Date: " << (date.empty() ? "Not specified" : date) << std::endl;
    std::cout << "Title: " << (title.empty() ? "Not specified" : title) << std::endl;
    std::cout << "Number of atoms: " << atoms.size() << std::endl;
    
    if (!atoms.empty()) {
        // Count unique chains
        std::vector<char> chains;
        for (const auto& atom : atoms) {
            if (std::find(chains.begin(), chains.end(), atom.chainID) == chains.end()) {
                chains.push_back(atom.chainID);
            }
        }
        std::cout << "Number of chains: " << chains.size() << std::endl;
        
        // Show coordinate ranges
        double minX = atoms[0].x, maxX = atoms[0].x;
        double minY = atoms[0].y, maxY = atoms[0].y;
        double minZ = atoms[0].z, maxZ = atoms[0].z;
        
        for (const auto& atom : atoms) {
            minX = std::min(minX, atom.x);
            maxX = std::max(maxX, atom.x);
            minY = std::min(minY, atom.y);
            maxY = std::max(maxY, atom.y);
            minZ = std::min(minZ, atom.z);
            maxZ = std::max(maxZ, atom.z);
        }
        
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Coordinate ranges:" << std::endl;
        std::cout << "  X: " << minX << " to " << maxX << " Å" << std::endl;
        std::cout << "  Y: " << minY << " to " << maxY << " Å" << std::endl;
        std::cout << "  Z: " << minZ << " to " << maxZ << " Å" << std::endl;
    }
}

// Print detailed atom information
void PDBData::printAtoms() const {
    if (atoms.empty()) {
        std::cout << "No atoms to display." << std::endl;
        return;
    }
    
    std::cout << "\n=== Atom Details ===" << std::endl;
    std::cout << std::setw(6) << "Serial" 
              << std::setw(5) << "Name" 
              << std::setw(4) << "Res" 
              << std::setw(2) << "Ch" 
              << std::setw(5) << "ResNo"
              << std::setw(10) << "X" 
              << std::setw(10) << "Y" 
              << std::setw(10) << "Z"
              << std::setw(8) << "Occ"
              << std::setw(8) << "TempF"
              << std::setw(4) << "Elem" << std::endl;
    
    std::cout << std::string(80, '-') << std::endl;
    
    size_t maxPrint = std::min(static_cast<size_t>(20), atoms.size());
    
    for (size_t i = 0; i < maxPrint; ++i) {
        const Atom& atom = atoms[i];
        std::cout << std::setw(6) << atom.serial
                  << std::setw(5) << atom.name
                  << std::setw(4) << atom.resName
                  << std::setw(2) << atom.chainID
                  << std::setw(5) << atom.resSeq
                  << std::setw(10) << std::fixed << std::setprecision(3) << atom.x
                  << std::setw(10) << std::fixed << std::setprecision(3) << atom.y
                  << std::setw(10) << std::fixed << std::setprecision(3) << atom.z
                  << std::setw(8) << std::fixed << std::setprecision(2) << atom.occupancy
                  << std::setw(8) << std::fixed << std::setprecision(2) << atom.tempFactor
                  << std::setw(4) << atom.element << std::endl;
    }
    
    if (atoms.size() > maxPrint) {
        std::cout << "... and " << (atoms.size() - maxPrint) << " more atoms" << std::endl;
    }
}

// Print usage information
void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " <pdb_file>" << std::endl;
    std::cout << "Example: " << programName << " protein.pdb" << std::endl;
}