#pragma once

#include "atom.h"
#include <vector>
#include <string>
#include <memory>

namespace biomesh {

/**
 * @brief Structure representing a bounding box
 */
struct BoundingBox {
    double min_x, min_y, min_z;    ///< Minimum coordinates
    double max_x, max_y, max_z;    ///< Maximum coordinates

    BoundingBox() : min_x(0), min_y(0), min_z(0), max_x(0), max_y(0), max_z(0) {}

    /**
     * @brief Get width (x dimension)
     * @return Width in Angstroms
     */
    double width() const { return max_x - min_x; }

    /**
     * @brief Get height (y dimension)
     * @return Height in Angstroms
     */
    double height() const { return max_y - min_y; }

    /**
     * @brief Get depth (z dimension)
     * @return Depth in Angstroms
     */
    double depth() const { return max_z - min_z; }

    /**
     * @brief Get center point
     * @param center_x Output center x coordinate
     * @param center_y Output center y coordinate
     * @param center_z Output center z coordinate
     */
    void getCenter(double& center_x, double& center_y, double& center_z) const {
        center_x = (min_x + max_x) / 2.0;
        center_y = (min_y + max_y) / 2.0;
        center_z = (min_z + max_z) / 2.0;
    }

    /**
     * @brief Get bounding sphere radius
     * @return Radius of smallest sphere containing the bounding box
     */
    double getBoundingSphereRadius() const {
        double cx, cy, cz;
        getCenter(cx, cy, cz);
        double dx = std::max(std::abs(max_x - cx), std::abs(min_x - cx));
        double dy = std::max(std::abs(max_y - cy), std::abs(min_y - cy));
        double dz = std::max(std::abs(max_z - cz), std::abs(min_z - cz));
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }

    /**
     * @brief Check if a point is within the bounding box
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @return True if point is within bounds
     */
    bool contains(double x, double y, double z) const {
        return x >= min_x && x <= max_x &&
               y >= min_y && y <= max_y &&
               z >= min_z && z <= max_z;
    }
};

/**
 * @brief Class representing a molecular chain
 */
class Molecule {
private:
    std::string chain_id_;
    std::vector<Atom> atoms_;
    mutable bool bounding_box_cached_;
    mutable BoundingBox bounding_box_;

public:
    /**
     * @brief Constructor
     * @param chain_id Chain identifier
     */
    explicit Molecule(const std::string& chain_id);

    /**
     * @brief Copy constructor
     */
    Molecule(const Molecule& other);

    /**
     * @brief Move constructor
     */
    Molecule(Molecule&& other) noexcept;

    /**
     * @brief Copy assignment operator
     */
    Molecule& operator=(const Molecule& other);

    /**
     * @brief Move assignment operator
     */
    Molecule& operator=(Molecule&& other) noexcept;

    /**
     * @brief Get chain ID
     * @return Chain identifier
     */
    const std::string& getChainId() const { return chain_id_; }

    /**
     * @brief Add an atom to the molecule
     * @param atom Atom to add
     */
    void addAtom(const Atom& atom);

    /**
     * @brief Add an atom to the molecule (move version)
     * @param atom Atom to add
     */
    void addAtom(Atom&& atom);

    /**
     * @brief Get atom by index
     * @param index Atom index
     * @return Reference to atom
     */
    const Atom& getAtom(size_t index) const;

    /**
     * @brief Get mutable atom by index
     * @param index Atom index
     * @return Reference to atom
     */
    Atom& getAtom(size_t index);

    /**
     * @brief Get number of atoms
     * @return Number of atoms in molecule
     */
    size_t getAtomCount() const { return atoms_.size(); }

    /**
     * @brief Get all atoms
     * @return Vector of atoms
     */
    const std::vector<Atom>& getAtoms() const { return atoms_; }

    /**
     * @brief Clear all atoms
     */
    void clear();

    /**
     * @brief Check if molecule is empty
     * @return True if no atoms
     */
    bool empty() const { return atoms_.empty(); }

    /**
     * @brief Calculate and get bounding box
     * @return Bounding box of the molecule
     */
    const BoundingBox& getBoundingBox() const;

    /**
     * @brief Get center point of the molecule
     * @param center_x Output center x coordinate
     * @param center_y Output center y coordinate
     * @param center_z Output center z coordinate
     */
    void getCenter(double& center_x, double& center_y, double& center_z) const;

    /**
     * @brief Get bounding sphere radius
     * @return Radius of smallest sphere containing the molecule
     */
    double getBoundingSphereRadius() const;

    /**
     * @brief Check if a point is within the molecule's bounding domain
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @return True if point is within bounds
     */
    bool pointInBounds(double x, double y, double z) const;

    /**
     * @brief Force recalculation of bounding box on next access
     */
    void invalidateBoundingBox();

private:
    /**
     * @brief Calculate bounding box from atoms
     */
    void calculateBoundingBox() const;
};

} // namespace biomesh