#pragma once

#include <string>
#include <ostream>

namespace pdb {

/**
 * @brief Represents an atom with its type and 3D coordinates
 * 
 * This class stores atomic information extracted from PDB files,
 * including the atom type and its Cartesian coordinates.
 */
class Atom {
public:
    /**
     * @brief Constructs an Atom with specified properties
     * @param id Sequential identifier for the atom
     * @param type Atom type (e.g., "C", "N", "O", "CA")
     * @param x X-coordinate in Angstroms
     * @param y Y-coordinate in Angstroms
     * @param z Z-coordinate in Angstroms
     */
    Atom(int id, std::string type, double x, double y, double z);

    // Getters
    int getId() const noexcept { return id_; }
    const std::string& getType() const noexcept { return type_; }
    double getX() const noexcept { return x_; }
    double getY() const noexcept { return y_; }
    double getZ() const noexcept { return z_; }

    /**
     * @brief Returns the distance from origin
     * @return Distance from (0,0,0) to this atom's position
     */
    double distanceFromOrigin() const noexcept;

    /**
     * @brief Calculates distance to another atom
     * @param other The other atom
     * @return Distance between this atom and the other atom
     */
    double distanceTo(const Atom& other) const noexcept;

    /**
     * @brief Equality comparison operator
     */
    bool operator==(const Atom& other) const noexcept;

    /**
     * @brief Stream output operator for easy printing
     */
    friend std::ostream& operator<<(std::ostream& os, const Atom& atom);

private:
    int id_;
    std::string type_;
    double x_, y_, z_;
};

} // namespace pdb