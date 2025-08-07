#pragma once

/**
 * @file biomesh.h
 * @brief Main header file for BioMesh library
 * 
 * BioMesh is a software tool designed to generate volumetric meshes built with 
 * hexahedrons (cubes) by recursive subdivision of space based on an octree algorithm.
 * 
 * This header provides access to all core BioMesh components including:
 * - Molecular hierarchy system with Atom and Molecule classes
 * - PDB file reading and parsing capabilities
 * - Atomic specification system for chemical properties
 * - Builder pattern for atom construction
 * - Bounding domain calculations
 */

#include "atom.h"
#include "molecule.h"
#include "atomic_spec.h"
#include "atomic_spec_table.h"
#include "atom_builder.h"
#include "pdb_reader.h"
#include "exceptions.h"

namespace biomesh {

/**
 * @brief Get version information
 * @return Version string
 */
inline const char* getVersion() {
    return "1.0.0";
}

/**
 * @brief Create a default atomic specification table
 * @return Shared pointer to populated AtomicSpecTable
 */
inline std::shared_ptr<AtomicSpecTable> createDefaultSpecTable() {
    return std::make_shared<AtomicSpecTable>();
}

} // namespace biomesh