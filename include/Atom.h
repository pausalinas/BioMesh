#pragma once

#include <string>

namespace BioMesh {

/**
 * @brief Represents a single atom with its coordinates and properties
 * 
 * This class stores basic atomic information including coordinates,
 * element type, and atom name as typically found in PDB files.
 */
class Atom {
public:
    /**
     * @brief Construct a new Atom object
     * 
     * @param x X coordinate
     * @param y Y coordinate 
     * @param z Z coordinate
     * @param element Element symbol (e.g., "C", "N", "O")
     * @param name Atom name (e.g., "CA", "CB")
     */
    Atom(double x, double y, double z, const std::string& element, const std::string& name);

    /**
     * @brief Default constructor
     */
    Atom() = default;

    /**
     * @brief Copy constructor
     */
    Atom(const Atom& other) = default;

    /**
     * @brief Assignment operator
     */
    Atom& operator=(const Atom& other) = default;

    /**
     * @brief Get X coordinate
     * @return double X coordinate
     */
    double getX() const { return x_; }

    /**
     * @brief Get Y coordinate
     * @return double Y coordinate
     */
    double getY() const { return y_; }

    /**
     * @brief Get Z coordinate
     * @return double Z coordinate
     */
    double getZ() const { return z_; }

    /**
     * @brief Get element symbol
     * @return const std::string& Element symbol
     */
    const std::string& getElement() const { return element_; }

    /**
     * @brief Get atom name
     * @return const std::string& Atom name
     */
    const std::string& getName() const { return name_; }

    /**
     * @brief Set coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     */
    void setCoordinates(double x, double y, double z);

    /**
     * @brief Set element symbol
     * @param element Element symbol
     */
    void setElement(const std::string& element);

    /**
     * @brief Set atom name
     * @param name Atom name
     */
    void setName(const std::string& name);

private:
    double x_ = 0.0;      ///< X coordinate
    double y_ = 0.0;      ///< Y coordinate
    double z_ = 0.0;      ///< Z coordinate
    std::string element_; ///< Element symbol
    std::string name_;    ///< Atom name
};

} // namespace BioMesh