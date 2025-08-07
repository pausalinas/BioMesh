#pragma once

#include <string>
#include <stdexcept>

namespace biomesh {

/**
 * @brief Exception thrown when PDB parsing fails
 */
class PDBParseException : public std::runtime_error {
public:
    explicit PDBParseException(const std::string& message)
        : std::runtime_error("PDB Parse Error: " + message) {}
};

/**
 * @brief Exception thrown when PDB file operations fail
 */
class PDBFileException : public std::runtime_error {
public:
    explicit PDBFileException(const std::string& message)
        : std::runtime_error("PDB File Error: " + message) {}
};

} // namespace biomesh