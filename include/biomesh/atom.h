#pragma once

#include <string>
#include <cmath>

namespace biomesh {

/**
 * @brief Structure representing an atom with enhanced properties
 */
struct Atom {
    // Basic PDB properties
    int serial_number;             ///< Atom serial number
    std::string atom_name;         ///< Atom name (e.g., "CA", "N", "O")
    std::string residue_name;      ///< Residue name (e.g., "ALA", "GLY")
    std::string chain_id;          ///< Chain identifier
    int residue_number;            ///< Residue sequence number
    double x, y, z;                ///< Atomic coordinates in Angstroms

    // Enhanced chemical properties
    std::string chemical_element;  ///< Chemical element symbol (e.g., "C", "N", "O")
    double atomic_radius;          ///< Atomic radius in Angstroms
    double atomic_mass;            ///< Atomic mass in atomic mass units (u)

    /**
     * @brief Default constructor
     */
    Atom() : serial_number(0), residue_number(0), x(0.0), y(0.0), z(0.0),
             atomic_radius(0.0), atomic_mass(0.0) {}

    /**
     * @brief Constructor with basic PDB properties (backward compatibility)
     * @param serial Serial number
     * @param name Atom name
     * @param res_name Residue name
     * @param chain Chain identifier
     * @param res_num Residue number
     * @param coord_x X coordinate
     * @param coord_y Y coordinate
     * @param coord_z Z coordinate
     */
    Atom(int serial, const std::string& name, const std::string& res_name,
         const std::string& chain, int res_num, double coord_x, double coord_y, double coord_z)
        : serial_number(serial), atom_name(name), residue_name(res_name),
          chain_id(chain), residue_number(res_num), x(coord_x), y(coord_y), z(coord_z),
          chemical_element(""), atomic_radius(0.0), atomic_mass(0.0) {}

    /**
     * @brief Constructor with enhanced properties
     * @param serial Serial number
     * @param name Atom name
     * @param res_name Residue name
     * @param chain Chain identifier
     * @param res_num Residue number
     * @param coord_x X coordinate
     * @param coord_y Y coordinate
     * @param coord_z Z coordinate
     * @param element Chemical element
     * @param radius Atomic radius
     * @param mass Atomic mass
     */
    Atom(int serial, const std::string& name, const std::string& res_name,
         const std::string& chain, int res_num, double coord_x, double coord_y, double coord_z,
         const std::string& element, double radius, double mass)
        : serial_number(serial), atom_name(name), residue_name(res_name),
          chain_id(chain), residue_number(res_num), x(coord_x), y(coord_y), z(coord_z),
          chemical_element(element), atomic_radius(radius), atomic_mass(mass) {}

    /**
     * @brief Get distance to another atom
     * @param other Other atom
     * @return Distance in Angstroms
     */
    double distanceTo(const Atom& other) const {
        double dx = x - other.x;
        double dy = y - other.y;
        double dz = z - other.z;
        return std::sqrt(dx*dx + dy*dy + dz*dz);
    }
};

} // namespace biomesh