#pragma once

#include "Atom.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <stdexcept>

namespace BioMesh {

/**
 * @brief Atomic specification containing element properties
 */
struct AtomicSpec {
    std::string elementSymbol;  ///< Chemical element symbol
    double radius;              ///< Atomic radius in Angstroms
    double mass;                ///< Atomic mass in Daltons

    AtomicSpec() = default;
    AtomicSpec(const std::string& symbol, double r, double m) 
        : elementSymbol(symbol), radius(r), mass(m) {}
};

/**
 * @brief Builder class for creating fully initialized Atom objects with atomic properties
 * 
 * AtomBuilder takes a vector of parsed Atom objects (with only chemical element and coordinates)
 * and looks up atomic properties from a specification table to create fully initialized atoms.
 */
class AtomBuilder {
public:
    /**
     * @brief Constructor that initializes the atomic specification table
     */
    AtomBuilder();

    /**
     * @brief Build fully initialized atoms from parsed atoms
     * @param parsedAtoms Vector of atoms with chemical element and coordinates
     * @return Vector of fully initialized atoms with radius and mass assigned
     * @throws std::runtime_error if an element is not found in the specification table
     */
    std::vector<Atom> buildAtoms(const std::vector<Atom>& parsedAtoms) const;

    /**
     * @brief Add or update an atomic specification
     * @param element Chemical element symbol
     * @param radius Atomic radius in Angstroms
     * @param mass Atomic mass in Daltons
     */
    void addAtomicSpec(const std::string& element, double radius, double mass);

    /**
     * @brief Check if an element exists in the specification table
     * @param element Chemical element symbol
     * @return true if element exists, false otherwise
     */
    bool hasElement(const std::string& element) const;

    /**
     * @brief Get atomic specification for an element
     * @param element Chemical element symbol
     * @return AtomicSpec for the element
     * @throws std::runtime_error if element not found
     */
    const AtomicSpec& getAtomicSpec(const std::string& element) const;

private:
    /**
     * @brief Initialize the default atomic specification table
     */
    void initializeAtomicSpecs();

    std::unordered_map<std::string, AtomicSpec> atomicSpecs_;  ///< Atomic specification table
};

} // namespace BioMesh