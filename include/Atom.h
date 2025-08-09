#pragma once

#include <string>
#include <array>

namespace BioMesh {

/**
 * @brief Represents an atom with position, chemical element, atomic radius, and atomic mass
 * 
 * The Atom class stores atomic properties including 3D coordinates, chemical element symbol,
 * atomic radius in Angstroms, and atomic mass in Daltons. Provides multiple constructors
 * for different initialization scenarios.
 */
class Atom {
public:
    // Type alias for coordinates
    using Coordinates = std::array<double, 3>;

    /**
     * @brief Default constructor
     */
    Atom() = default;

    /**
     * @brief Constructor with coordinates and chemical element
     * @param x X coordinate
     * @param y Y coordinate  
     * @param z Z coordinate
     * @param chemicalElement Chemical element symbol (e.g., "C", "N", "O")
     */
    Atom(double x, double y, double z, const std::string& chemicalElement);

    /**
     * @brief Constructor with only chemical element
     * @param chemicalElement Chemical element symbol (e.g., "C", "N", "O")
     */
    explicit Atom(const std::string& chemicalElement);

    /**
     * @brief Constructor with chemical element and atomic radius
     * @param chemicalElement Chemical element symbol (e.g., "C", "N", "O")
     * @param atomicRadius Atomic radius in Angstroms
     */
    Atom(const std::string& chemicalElement, double atomicRadius);

    /**
     * @brief Constructor with chemical element, atomic radius, and atomic mass
     * @param chemicalElement Chemical element symbol (e.g., "C", "N", "O")
     * @param atomicRadius Atomic radius in Angstroms
     * @param atomicMass Atomic mass in Daltons
     */
    Atom(const std::string& chemicalElement, double atomicRadius, double atomicMass);

    // Getters
    const Coordinates& getCoordinates() const { return coordinates_; }
    double getX() const { return coordinates_[0]; }
    double getY() const { return coordinates_[1]; }
    double getZ() const { return coordinates_[2]; }
    const std::string& getChemicalElement() const { return chemicalElement_; }
    double getAtomicRadius() const { return atomicRadius_; }
    double getAtomicMass() const { return atomicMass_; }

    // Setters
    void setCoordinates(double x, double y, double z);
    void setCoordinates(const Coordinates& coords) { coordinates_ = coords; }
    void setX(double x) { coordinates_[0] = x; }
    void setY(double y) { coordinates_[1] = y; }
    void setZ(double z) { coordinates_[2] = z; }
    void setChemicalElement(const std::string& element) { chemicalElement_ = element; }
    void setAtomicRadius(double radius) { atomicRadius_ = radius; }
    void setAtomicMass(double mass) { atomicMass_ = mass; }

private:
    Coordinates coordinates_{0.0, 0.0, 0.0};  ///< 3D coordinates of the atom
    std::string chemicalElement_;              ///< Chemical element symbol
    double atomicRadius_{0.0};                 ///< Atomic radius in Angstroms
    double atomicMass_{0.0};                   ///< Atomic mass in Daltons
};

} // namespace BioMesh