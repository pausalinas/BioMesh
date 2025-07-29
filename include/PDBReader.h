#pragma once

#include "Atom.h"
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>

namespace BioMesh {

/**
 * @brief Exception class for PDB parsing errors
 */
class PDBParseException : public std::runtime_error {
public:
    explicit PDBParseException(const std::string& message) 
        : std::runtime_error("PDB Parse Error: " + message) {}
};

/**
 * @brief Exception class for PDB file access errors
 */
class PDBFileException : public std::runtime_error {
public:
    explicit PDBFileException(const std::string& message)
        : std::runtime_error("PDB File Error: " + message) {}
};

/**
 * @brief Class responsible for reading and parsing PDB structure files
 * 
 * This class follows RAII principles and uses smart pointers to manage resources.
 * It processes only ATOM lines and stores the parsed data in STL containers.
 */
class PDBReader {
private:
    std::vector<std::unique_ptr<Atom>> atoms_;  ///< Container for parsed atoms
    std::string filename_;                       ///< Name of the PDB file
    int atom_counter_;                          ///< Sequential counter for atoms

    /**
     * @brief Parse a single ATOM line from PDB file
     * @param line The line to parse
     * @return Unique pointer to the parsed Atom object
     * @throws PDBParseException if the line cannot be parsed
     */
    std::unique_ptr<Atom> parseAtomLine(const std::string& line);

    /**
     * @brief Safely extract substring with bounds checking
     * @param str Source string
     * @param start Start position (0-based)
     * @param length Length of substring
     * @return Extracted and trimmed substring
     */
    std::string safeSubstring(const std::string& str, size_t start, size_t length) const;

    /**
     * @brief Trim whitespace from string
     * @param str String to trim
     * @return Trimmed string
     */
    std::string trim(const std::string& str) const;

public:
    /**
     * @brief Constructor with initialization list
     * @param filename Path to the PDB file
     */
    explicit PDBReader(const std::string& filename);

    /**
     * @brief Destructor (default, smart pointers handle cleanup)
     */
    ~PDBReader() = default;

    // Disable copy constructor and assignment (following RAII principles)
    PDBReader(const PDBReader&) = delete;
    PDBReader& operator=(const PDBReader&) = delete;

    // Enable move constructor and assignment
    PDBReader(PDBReader&&) = default;
    PDBReader& operator=(PDBReader&&) = default;

    /**
     * @brief Parse the PDB file and extract ATOM records
     * @throws PDBFileException if file cannot be opened or read
     * @throws PDBParseException if parsing fails
     */
    void parse();

    /**
     * @brief Get const reference to the parsed atoms
     * @return Const reference to vector of unique_ptr<Atom>
     */
    const std::vector<std::unique_ptr<Atom>>& getAtoms() const;

    /**
     * @brief Get the number of parsed atoms
     * @return Number of atoms
     */
    size_t getAtomCount() const;

    /**
     * @brief Get the filename being processed
     * @return Filename string
     */
    const std::string& getFilename() const;

    /**
     * @brief Clear all parsed atoms
     */
    void clear();
};

} // namespace BioMesh