#pragma once

#include "atom.h"
#include "atomic_spec_table.h"
#include <memory>
#include <string>

namespace biomesh {

/**
 * @brief Builder class for constructing atoms with automatic property assignment
 */
class AtomBuilder {
private:
    Atom atom_;
    std::shared_ptr<AtomicSpecTable> spec_table_;

public:
    /**
     * @brief Constructor with atomic specification table
     * @param spec_table Atomic specification table for property lookup
     */
    explicit AtomBuilder(std::shared_ptr<AtomicSpecTable> spec_table = nullptr);

    /**
     * @brief Set serial number
     * @param serial Serial number
     * @return Reference to this builder
     */
    AtomBuilder& setSerial(int serial);

    /**
     * @brief Set atom name
     * @param name Atom name
     * @return Reference to this builder
     */
    AtomBuilder& setAtomName(const std::string& name);

    /**
     * @brief Set residue name
     * @param name Residue name
     * @return Reference to this builder
     */
    AtomBuilder& setResidueName(const std::string& name);

    /**
     * @brief Set chain ID
     * @param chain Chain identifier
     * @return Reference to this builder
     */
    AtomBuilder& setChainId(const std::string& chain);

    /**
     * @brief Set residue number
     * @param number Residue number
     * @return Reference to this builder
     */
    AtomBuilder& setResidueNumber(int number);

    /**
     * @brief Set coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @return Reference to this builder
     */
    AtomBuilder& setCoordinates(double x, double y, double z);

    /**
     * @brief Set chemical element explicitly
     * @param element Chemical element symbol
     * @return Reference to this builder
     */
    AtomBuilder& setChemicalElement(const std::string& element);

    /**
     * @brief Set atomic radius explicitly
     * @param radius Atomic radius in Angstroms
     * @return Reference to this builder
     */
    AtomBuilder& setAtomicRadius(double radius);

    /**
     * @brief Set atomic mass explicitly
     * @param mass Atomic mass in u
     * @return Reference to this builder
     */
    AtomBuilder& setAtomicMass(double mass);

    /**
     * @brief Auto-detect chemical element from atom name and apply properties
     * @return Reference to this builder
     */
    AtomBuilder& autoDetectElement();

    /**
     * @brief Build the atom
     * @return Constructed atom
     */
    Atom build();

    /**
     * @brief Reset builder for reuse
     * @return Reference to this builder
     */
    AtomBuilder& reset();

private:
    /**
     * @brief Extract element symbol from atom name
     * @param atom_name Atom name from PDB
     * @return Detected element symbol
     */
    std::string extractElementFromName(const std::string& atom_name) const;

    /**
     * @brief Apply atomic properties from specification table
     * @param element Element symbol
     */
    void applySpecification(const std::string& element);
};

} // namespace biomesh