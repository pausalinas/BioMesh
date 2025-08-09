#pragma once

#include <vector>
#include <string>
#include <memory>
#include "Atom.h"

namespace BioMesh {

/**
 * @brief PDB file reader and parser
 * 
 * This class provides functionality to read and parse PDB (Protein Data Bank) files,
 * extracting atomic coordinates and properties. It follows RAII principles and
 * provides direct access to parsed atoms for mesh generation.
 */
class PDBReader {
public:
    /**
     * @brief Construct a new PDBReader object
     */
    PDBReader() = default;

    /**
     * @brief Destructor
     */
    ~PDBReader() = default;

    /**
     * @brief Copy constructor (deleted - move semantics preferred)
     */
    PDBReader(const PDBReader&) = delete;

    /**
     * @brief Assignment operator (deleted - move semantics preferred)
     */
    PDBReader& operator=(const PDBReader&) = delete;

    /**
     * @brief Move constructor
     */
    PDBReader(PDBReader&&) = default;

    /**
     * @brief Move assignment operator
     */
    PDBReader& operator=(PDBReader&&) = default;

    /**
     * @brief Read and parse a PDB file
     * 
     * @param filename Path to the PDB file
     * @return true if successful, false otherwise
     */
    bool readFile(const std::string& filename);

    /**
     * @brief Parse PDB content from a string
     * 
     * @param pdbContent PDB file content as string
     * @return true if successful, false otherwise
     */
    bool parseContent(const std::string& pdbContent);

    /**
     * @brief Get the parsed atoms
     * 
     * @return const std::vector<Atom>& Vector of parsed atoms
     */
    const std::vector<Atom>& getAtoms() const { return atoms_; }

    /**
     * @brief Get number of atoms
     * 
     * @return size_t Number of atoms
     */
    size_t getAtomCount() const { return atoms_.size(); }

    /**
     * @brief Clear all parsed data
     */
    void clear();

    /**
     * @brief Check if any atoms have been loaded
     * 
     * @return true if atoms are loaded, false otherwise
     */
    bool hasAtoms() const { return !atoms_.empty(); }

private:
    std::vector<Atom> atoms_; ///< Container for parsed atoms

    /**
     * @brief Parse a single ATOM line from PDB format
     * 
     * @param line PDB ATOM line
     * @return std::unique_ptr<Atom> Parsed atom or nullptr if parsing failed
     */
    std::unique_ptr<Atom> parseAtomLine(const std::string& line);

    /**
     * @brief Extract and trim substring from fixed-width PDB fields
     * 
     * @param line Input line
     * @param start Start position
     * @param length Field length
     * @return std::string Trimmed substring
     */
    std::string extractField(const std::string& line, size_t start, size_t length);

    /**
     * @brief Trim whitespace from string
     * 
     * @param str Input string
     * @return std::string Trimmed string
     */
    std::string trim(const std::string& str);
};

} // namespace BioMesh