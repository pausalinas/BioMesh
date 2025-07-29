#include "PDBReader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace pdb {

PDBReader::AtomContainer PDBReader::readFile(const std::string& filename) {
    // Reset statistics
    atomCount_ = 0;
    linesProcessed_ = 0;

    // Use RAII for file management
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw PDBFileException("Cannot open file: " + filename);
    }

    if (file.bad()) {
        throw PDBFileException("File stream is in bad state: " + filename);
    }

    // Read entire file content
    std::ostringstream buffer;
    buffer << file.rdbuf();
    
    if (file.bad()) {
        throw PDBFileException("Error reading file: " + filename);
    }

    // File is automatically closed when ifstream goes out of scope (RAII)
    return parseContent(buffer.str());
}

PDBReader::AtomContainer PDBReader::parseContent(const std::string& content) {
    AtomContainer atoms;
    atomCount_ = 0;
    linesProcessed_ = 0;

    if (content.empty()) {
        return atoms; // Return empty container for empty content
    }

    std::istringstream stream(content);
    std::string line;
    int atomId = 1; // Sequential identifier starts from 1

    while (std::getline(stream, line)) {
        ++linesProcessed_;

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        // Process only ATOM lines
        if (isValidAtomLine(line)) {
            try {
                auto atom = parseAtomLine(line, atomId++);
                atoms.push_back(std::move(atom));
                ++atomCount_;
            } catch (const PDBParsingException& e) {
                // Re-throw with line number information
                throw PDBParsingException("Line " + std::to_string(linesProcessed_) + ": " + e.what());
            }
        }
        // Silently skip non-ATOM lines (HEADER, REMARK, etc.)
    }

    return atoms;
}

bool PDBReader::isValidAtomLine(const std::string& line) noexcept {
    // Check if line starts with "ATOM" and has minimum required length
    return line.length() >= 54 && 
           line.substr(0, 4) == "ATOM";
}

std::unique_ptr<Atom> PDBReader::parseAtomLine(const std::string& line, int atomId) {
    // PDB format specification for ATOM records:
    // Columns 1-6: Record name "ATOM  "
    // Columns 13-16: Atom name
    // Columns 31-38: X coordinate
    // Columns 39-46: Y coordinate 
    // Columns 47-54: Z coordinate

    if (line.length() < 54) {
        throw PDBParsingException("ATOM line too short (expected at least 54 characters, got " + 
                                std::to_string(line.length()) + ")");
    }

    try {
        // Extract atom type (columns 13-16, 0-based indexing 12-15)
        std::string atomType = trim(safeSubstring(line, 12, 4));
        if (atomType.empty()) {
            throw PDBParsingException("Empty atom type");
        }

        // Extract coordinates (using 0-based indexing)
        std::string xStr = trim(safeSubstring(line, 30, 8)); // columns 31-38
        std::string yStr = trim(safeSubstring(line, 38, 8)); // columns 39-46
        std::string zStr = trim(safeSubstring(line, 46, 8)); // columns 47-54

        // Convert coordinates to double with error checking
        double x = safeStringToDouble(xStr, "X coordinate");
        double y = safeStringToDouble(yStr, "Y coordinate");
        double z = safeStringToDouble(zStr, "Z coordinate");

        // Create atom using smart pointer
        return std::make_unique<Atom>(atomId, std::move(atomType), x, y, z);

    } catch (const std::exception& e) {
        throw PDBParsingException("Failed to parse ATOM line: " + std::string(e.what()));
    }
}

std::string PDBReader::safeSubstring(const std::string& str, size_t start, size_t length) const {
    if (start >= str.length()) {
        return "";
    }
    
    size_t actualLength = std::min(length, str.length() - start);
    return str.substr(start, actualLength);
}

std::string PDBReader::trim(const std::string& str) const {
    if (str.empty()) {
        return str;
    }

    // Find first non-whitespace character
    auto start = str.find_first_not_of(" \t\n\r");
    if (start == std::string::npos) {
        return ""; // String contains only whitespace
    }

    // Find last non-whitespace character
    auto end = str.find_last_not_of(" \t\n\r");
    
    return str.substr(start, end - start + 1);
}

double PDBReader::safeStringToDouble(const std::string& str, const std::string& fieldName) const {
    if (str.empty()) {
        throw PDBParsingException("Empty " + fieldName + " field");
    }

    try {
        size_t pos;
        double value = std::stod(str, &pos);
        
        // Check if entire string was consumed
        if (pos != str.length()) {
            throw PDBParsingException("Invalid " + fieldName + " format: '" + str + "'");
        }
        
        return value;
    } catch (const std::invalid_argument&) {
        throw PDBParsingException("Invalid " + fieldName + " format: '" + str + "'");
    } catch (const std::out_of_range&) {
        throw PDBParsingException(fieldName + " value out of range: '" + str + "'");
    }
}

} // namespace pdb