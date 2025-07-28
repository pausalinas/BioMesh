#include "pdb_reader.h"
#include <iomanip>

/**
 * Default constructor
 */
PDBReader::PDBReader() : filename(""), file_loaded(false) {
}

/**
 * Constructor with filename
 */
PDBReader::PDBReader(const std::string& filename) : filename(filename), file_loaded(false) {
    openFile(filename);
    parseFile();
}

/**
 * Destructor
 */
PDBReader::~PDBReader() {
}

/**
 * Open a PDB file
 */
bool PDBReader::openFile(const std::string& filename) {
    this->filename = filename;
    this->file_loaded = false;
    this->atoms.clear();
    this->header = "";
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    file.close();
    return true;
}

/**
 * Parse the PDB file
 */
bool PDBReader::parseFile() {
    if (filename.empty()) {
        std::cerr << "Error: No filename specified" << std::endl;
        return false;
    }
    
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return false;
    }
    
    std::string line;
    bool header_found = false;
    
    while (std::getline(file, line)) {
        if (line.length() < 6) continue;
        
        std::string record_type = line.substr(0, 6);
        record_type = trim(record_type);
        
        if (record_type == "HEADER" && !header_found) {
            header = line;
            header_found = true;
        }
        else if (record_type == "ATOM" || record_type == "HETATM") {
            try {
                PDBAtom atom = parseAtomLine(line);
                atoms.push_back(atom);
            } catch (const std::exception& e) {
                std::cerr << "Warning: Could not parse line: " << line << std::endl;
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
        else if (record_type == "END") {
            break;
        }
    }
    
    file.close();
    file_loaded = true;
    
    std::cout << "Successfully loaded PDB file: " << filename << std::endl;
    std::cout << "Number of atoms parsed: " << atoms.size() << std::endl;
    
    return true;
}

/**
 * Parse an ATOM or HETATM line
 */
PDBAtom PDBReader::parseAtomLine(const std::string& line) {
    PDBAtom atom;
    
    if (line.length() < 54) {
        throw std::runtime_error("Line too short for ATOM/HETATM record");
    }
    
    // Parse according to PDB format specification
    atom.record_name = trim(line.substr(0, 6));
    atom.atom_number = std::stoi(trim(line.substr(6, 5)));
    atom.atom_name = trim(line.substr(12, 4));
    atom.alt_loc = line.length() > 16 ? line[16] : ' ';
    atom.residue_name = trim(line.substr(17, 3));
    atom.chain_id = line.length() > 21 ? line[21] : ' ';
    atom.residue_number = std::stoi(trim(line.substr(22, 4)));
    atom.insertion_code = line.length() > 26 ? line[26] : ' ';
    
    // Parse coordinates
    atom.x = std::stod(trim(line.substr(30, 8)));
    atom.y = std::stod(trim(line.substr(38, 8)));
    atom.z = std::stod(trim(line.substr(46, 8)));
    
    // Parse occupancy and temperature factor if available
    if (line.length() >= 60) {
        atom.occupancy = std::stod(trim(line.substr(54, 6)));
    } else {
        atom.occupancy = 1.0;
    }
    
    if (line.length() >= 66) {
        atom.temp_factor = std::stod(trim(line.substr(60, 6)));
    } else {
        atom.temp_factor = 0.0;
    }
    
    // Parse element and charge if available
    if (line.length() >= 78) {
        atom.element = trim(line.substr(76, 2));
    }
    
    if (line.length() >= 80) {
        atom.charge = trim(line.substr(78, 2));
    }
    
    return atom;
}

/**
 * Print all atoms
 */
void PDBReader::printAtoms() const {
    if (!file_loaded) {
        std::cout << "No file loaded." << std::endl;
        return;
    }
    
    std::cout << "\n=== ATOM RECORDS ===" << std::endl;
    std::cout << std::left << std::setw(6) << "Type" 
              << std::setw(6) << "Num" 
              << std::setw(6) << "Name" 
              << std::setw(4) << "Res" 
              << std::setw(2) << "Ch" 
              << std::setw(5) << "ResN" 
              << std::setw(10) << "X" 
              << std::setw(10) << "Y" 
              << std::setw(10) << "Z" << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    for (const auto& atom : atoms) {
        std::cout << std::left << std::setw(6) << atom.record_name
                  << std::setw(6) << atom.atom_number
                  << std::setw(6) << atom.atom_name
                  << std::setw(4) << atom.residue_name
                  << std::setw(2) << atom.chain_id
                  << std::setw(5) << atom.residue_number
                  << std::fixed << std::setprecision(3)
                  << std::setw(10) << atom.x
                  << std::setw(10) << atom.y
                  << std::setw(10) << atom.z << std::endl;
    }
}

/**
 * Print summary information
 */
void PDBReader::printSummary() const {
    if (!file_loaded) {
        std::cout << "No file loaded." << std::endl;
        return;
    }
    
    std::cout << "\n=== PDB FILE SUMMARY ===" << std::endl;
    std::cout << "Filename: " << filename << std::endl;
    std::cout << "Header: " << (header.empty() ? "Not found" : header) << std::endl;
    std::cout << "Total atoms: " << atoms.size() << std::endl;
    
    if (!atoms.empty()) {
        // Calculate bounds
        double min_x = atoms[0].x, max_x = atoms[0].x;
        double min_y = atoms[0].y, max_y = atoms[0].y;
        double min_z = atoms[0].z, max_z = atoms[0].z;
        
        for (const auto& atom : atoms) {
            min_x = std::min(min_x, atom.x);
            max_x = std::max(max_x, atom.x);
            min_y = std::min(min_y, atom.y);
            max_y = std::max(max_y, atom.y);
            min_z = std::min(min_z, atom.z);
            max_z = std::max(max_z, atom.z);
        }
        
        std::cout << "Coordinate bounds:" << std::endl;
        std::cout << "  X: " << std::fixed << std::setprecision(3) << min_x << " to " << max_x << std::endl;
        std::cout << "  Y: " << std::fixed << std::setprecision(3) << min_y << " to " << max_y << std::endl;
        std::cout << "  Z: " << std::fixed << std::setprecision(3) << min_z << " to " << max_z << std::endl;
    }
}

/**
 * Getters
 */
const std::vector<PDBAtom>& PDBReader::getAtoms() const {
    return atoms;
}

const std::string& PDBReader::getHeader() const {
    return header;
}

int PDBReader::getAtomCount() const {
    return atoms.size();
}

bool PDBReader::isLoaded() const {
    return file_loaded;
}

/**
 * Utility function to trim whitespace
 */
std::string PDBReader::trim(const std::string& str) {
    size_t first = str.find_first_not_of(' ');
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}