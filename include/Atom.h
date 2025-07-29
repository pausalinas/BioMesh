#pragma once

#include <string>

namespace BioMesh {

/**
 * @brief Represents an atom with its properties extracted from a PDB file
 */
struct Atom {
    int serial_number;           ///< Sequential identifier for the atom
    std::string atom_name;       ///< Atom name (e.g., "CA", "N", "O")
    std::string residue_name;    ///< Residue name (e.g., "ALA", "GLY")
    char chain_id;               ///< Chain identifier
    int residue_number;          ///< Residue sequence number
    double x;                    ///< X coordinate
    double y;                    ///< Y coordinate  
    double z;                    ///< Z coordinate

    /**
     * @brief Constructor with initialization list
     */
    Atom(int serial, const std::string& atom, const std::string& residue,
         char chain, int res_num, double x_coord, double y_coord, double z_coord)
        : serial_number(serial)
        , atom_name(atom)
        , residue_name(residue)
        , chain_id(chain)
        , residue_number(res_num)
        , x(x_coord)
        , y(y_coord)
        , z(z_coord) {}

    /**
     * @brief Default constructor
     */
    Atom() 
        : serial_number(0)
        , atom_name("")
        , residue_name("")
        , chain_id(' ')
        , residue_number(0)
        , x(0.0)
        , y(0.0)
        , z(0.0) {}
};

} // namespace BioMesh