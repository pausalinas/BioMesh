#pragma once

#include "atom.h"
#include "molecule.h"
#include "atom_builder.h"
#include "atomic_spec_table.h"
#include "exceptions.h"
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>

namespace biomesh {

/**
 * @brief Class for reading and parsing PDB files
 */
class PDBReader {
private:
    std::unordered_map<std::string, std::unique_ptr<Molecule>> molecules_;
    std::vector<Atom> all_atoms_;  // Flat list for backward compatibility
    std::shared_ptr<AtomicSpecTable> spec_table_;

public:
    /**
     * @brief Constructor with optional atomic specification table
     * @param spec_table Atomic specification table for property assignment
     */
    explicit PDBReader(std::shared_ptr<AtomicSpecTable> spec_table = nullptr);

    /**
     * @brief Read and parse a PDB file
     * @param filename Path to PDB file
     * @throws PDBFileException if file cannot be opened
     * @throws PDBParseException if parsing fails
     */
    void readFile(const std::string& filename);

    /**
     * @brief Parse PDB content from string
     * @param content PDB file content
     * @throws PDBParseException if parsing fails
     */
    void parseContent(const std::string& content);

    /**
     * @brief Get molecule by chain ID
     * @param chain_id Chain identifier
     * @return Pointer to molecule or nullptr if not found
     */
    const Molecule* getMolecule(const std::string& chain_id) const;

    /**
     * @brief Get all molecules
     * @return Map of chain ID to molecule
     */
    const std::unordered_map<std::string, std::unique_ptr<Molecule>>& getMolecules() const {
        return molecules_;
    }

    /**
     * @brief Get all atoms (flat list for backward compatibility)
     * @return Vector of all atoms
     */
    const std::vector<Atom>& getAllAtoms() const { return all_atoms_; }

    /**
     * @brief Get number of molecules
     * @return Number of unique chains/molecules
     */
    size_t getMoleculeCount() const { return molecules_.size(); }

    /**
     * @brief Get total number of atoms across all molecules
     * @return Total atom count
     */
    size_t getTotalAtomCount() const { return all_atoms_.size(); }

    /**
     * @brief Clear all loaded data
     */
    void clear();

    /**
     * @brief Check if any data is loaded
     * @return True if molecules exist
     */
    bool hasData() const { return !molecules_.empty(); }

    /**
     * @brief Get list of chain IDs
     * @return Vector of chain identifiers
     */
    std::vector<std::string> getChainIds() const;

    /**
     * @brief Set atomic specification table
     * @param spec_table New specification table
     */
    void setAtomicSpecTable(std::shared_ptr<AtomicSpecTable> spec_table);

private:
    /**
     * @brief Parse a single ATOM line from PDB
     * @param line PDB ATOM line
     * @return Parsed atom
     * @throws PDBParseException if line format is invalid
     */
    Atom parseAtomLine(const std::string& line);

    /**
     * @brief Get or create molecule for given chain ID
     * @param chain_id Chain identifier
     * @return Reference to molecule
     */
    Molecule& getOrCreateMolecule(const std::string& chain_id);

    /**
     * @brief Trim whitespace from string
     * @param str String to trim
     * @return Trimmed string
     */
    std::string trim(const std::string& str) const;

    /**
     * @brief Extract substring from PDB line with bounds checking
     * @param line PDB line
     * @param start Start position (0-based)
     * @param length Length of substring
     * @return Extracted and trimmed substring
     */
    std::string extractField(const std::string& line, size_t start, size_t length) const;

    /**
     * @brief Parse integer from string with error handling
     * @param str String to parse
     * @param field_name Field name for error messages
     * @return Parsed integer
     * @throws PDBParseException if parsing fails
     */
    int parseInt(const std::string& str, const std::string& field_name) const;

    /**
     * @brief Parse double from string with error handling
     * @param str String to parse
     * @param field_name Field name for error messages
     * @return Parsed double
     * @throws PDBParseException if parsing fails
     */
    double parseDouble(const std::string& str, const std::string& field_name) const;
};

} // namespace biomesh