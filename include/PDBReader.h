#pragma once

#include "Atom.h"
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

namespace pdb {

/**
 * @brief Custom exception for PDB file parsing errors
 */
class PDBParsingException : public std::runtime_error {
public:
    explicit PDBParsingException(const std::string& message)
        : std::runtime_error("PDB Parsing Error: " + message) {}
};

/**
 * @brief Custom exception for file I/O errors
 */
class PDBFileException : public std::runtime_error {
public:
    explicit PDBFileException(const std::string& message)
        : std::runtime_error("PDB File Error: " + message) {}
};

/**
 * @brief Reader class for parsing PDB structure files
 * 
 * This class is responsible for reading PDB files and extracting
 * atom information. It follows RAII principles and uses smart pointers
 * to manage resources safely.
 */
class PDBReader {
public:
    using AtomContainer = std::vector<std::unique_ptr<Atom>>;

    /**
     * @brief Default constructor
     */
    PDBReader() = default;

    /**
     * @brief Destructor - compiler generated is sufficient due to RAII
     */
    ~PDBReader() = default;

    // Disable copy operations to avoid issues with unique_ptr
    PDBReader(const PDBReader&) = delete;
    PDBReader& operator=(const PDBReader&) = delete;

    // Enable move operations
    PDBReader(PDBReader&&) = default;
    PDBReader& operator=(PDBReader&&) = default;

    /**
     * @brief Reads and parses a PDB file
     * @param filename Path to the PDB file
     * @return Vector of unique_ptr to Atom objects
     * @throws PDBFileException if file cannot be opened or read
     * @throws PDBParsingException if parsing fails
     */
    AtomContainer readFile(const std::string& filename);

    /**
     * @brief Parses PDB content from a string
     * @param content PDB file content as string
     * @return Vector of unique_ptr to Atom objects
     * @throws PDBParsingException if parsing fails
     */
    AtomContainer parseContent(const std::string& content);

    /**
     * @brief Gets the number of atoms read in the last operation
     * @return Number of atoms parsed
     */
    size_t getAtomCount() const noexcept { return atomCount_; }

    /**
     * @brief Gets the number of lines processed in the last operation
     * @return Number of lines processed
     */
    size_t getLinesProcessed() const noexcept { return linesProcessed_; }

    /**
     * @brief Validates if a line is a valid ATOM line
     * @param line PDB line to validate
     * @return true if line is valid ATOM line, false otherwise
     */
    static bool isValidAtomLine(const std::string& line) noexcept;

private:
    /**
     * @brief Parses a single ATOM line from PDB format
     * @param line The ATOM line to parse
     * @param atomId Sequential ID for the atom
     * @return Unique pointer to parsed Atom
     * @throws PDBParsingException if line format is invalid
     */
    std::unique_ptr<Atom> parseAtomLine(const std::string& line, int atomId);

    /**
     * @brief Extracts substring safely with bounds checking
     * @param str Source string
     * @param start Start position (0-based)
     * @param length Length of substring
     * @return Extracted and trimmed substring
     */
    std::string safeSubstring(const std::string& str, size_t start, size_t length) const;

    /**
     * @brief Trims whitespace from both ends of a string
     * @param str String to trim
     * @return Trimmed string
     */
    std::string trim(const std::string& str) const;

    /**
     * @brief Safely converts string to double
     * @param str String to convert
     * @param fieldName Name of field for error reporting
     * @return Converted double value
     * @throws PDBParsingException if conversion fails
     */
    double safeStringToDouble(const std::string& str, const std::string& fieldName) const;

    // Statistics from last parsing operation
    size_t atomCount_ = 0;
    size_t linesProcessed_ = 0;
};

} // namespace pdb