#include "PDBReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace BioMesh {

PDBReader::PDBReader(const std::string& filename) 
    : filename_(filename)
    , atom_counter_(0) {
    atoms_.reserve(1000); // Reserve space for typical protein size
}

void PDBReader::parse() {
    std::ifstream file(filename_);
    
    if (!file.is_open()) {
        throw PDBFileException("Cannot open file: " + filename_);
    }

    // RAII: file will be automatically closed when destructor is called
    std::string line;
    int line_number = 0;
    
    try {
        while (std::getline(file, line)) {
            ++line_number;
            
            // Only process ATOM lines
            if (line.length() >= 6 && line.substr(0, 6) == "ATOM  ") {
                try {
                    auto atom = parseAtomLine(line);
                    if (atom) {
                        atoms_.push_back(std::move(atom));
                        ++atom_counter_;
                    }
                } catch (const PDBParseException& e) {
                    // Add line number context to the exception
                    throw PDBParseException("Line " + std::to_string(line_number) + ": " + e.what());
                }
            }
        }
        
        if (file.bad()) {
            throw PDBFileException("Error reading file: " + filename_);
        }
        
    } catch (const std::exception& e) {
        file.close();
        throw;
    }
}

std::unique_ptr<Atom> PDBReader::parseAtomLine(const std::string& line) {
    if (line.length() < 54) {
        throw PDBParseException("ATOM line too short (minimum 54 characters required)");
    }

    try {
        // Parse according to PDB format specification
        // ATOM      1  N   ALA A   1      20.154  16.967  14.365  1.00 20.00           N
        //       1         2         3         4         5         6
        // 123456789012345678901234567890123456789012345678901234567890

        // Serial number (columns 7-11)
        std::string serial_str = safeSubstring(line, 6, 5);
        int serial_number = std::stoi(serial_str);

        // Atom name (columns 13-16)
        std::string atom_name = safeSubstring(line, 12, 4);

        // Residue name (columns 18-20)
        std::string residue_name = safeSubstring(line, 17, 3);

        // Chain ID (column 22)
        char chain_id = line.length() > 21 ? line[21] : ' ';

        // Residue number (columns 23-26)
        std::string res_num_str = safeSubstring(line, 22, 4);
        int residue_number = std::stoi(res_num_str);

        // Coordinates (columns 31-38, 39-46, 47-54)
        std::string x_str = safeSubstring(line, 30, 8);
        std::string y_str = safeSubstring(line, 38, 8);
        std::string z_str = safeSubstring(line, 46, 8);

        double x = std::stod(x_str);
        double y = std::stod(y_str);
        double z = std::stod(z_str);

        return std::make_unique<Atom>(serial_number, atom_name, residue_name, 
                                    chain_id, residue_number, x, y, z);

    } catch (const std::invalid_argument& e) {
        throw PDBParseException("Invalid numeric value in ATOM line");
    } catch (const std::out_of_range& e) {
        throw PDBParseException("Numeric value out of range in ATOM line");
    }
}

std::string PDBReader::safeSubstring(const std::string& str, size_t start, size_t length) const {
    if (start >= str.length()) {
        return "";
    }
    
    size_t actual_length = std::min(length, str.length() - start);
    return trim(str.substr(start, actual_length));
}

std::string PDBReader::trim(const std::string& str) const {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

const std::vector<std::unique_ptr<Atom>>& PDBReader::getAtoms() const {
    return atoms_;
}

size_t PDBReader::getAtomCount() const {
    return atoms_.size();
}

const std::string& PDBReader::getFilename() const {
    return filename_;
}

void PDBReader::clear() {
    atoms_.clear();
    atom_counter_ = 0;
}

} // namespace BioMesh