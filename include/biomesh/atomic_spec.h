#pragma once

#include <string>

namespace biomesh {

/**
 * @brief Structure representing atomic specification properties
 */
struct AtomicSpec {
    std::string element_symbol;    ///< Chemical element symbol (e.g., "C", "N", "O")
    double atomic_radius;          ///< Atomic radius in Angstroms
    double atomic_mass;            ///< Atomic mass in atomic mass units (u)

    /**
     * @brief Default constructor
     */
    AtomicSpec() : element_symbol(""), atomic_radius(0.0), atomic_mass(0.0) {}

    /**
     * @brief Constructor with all parameters
     * @param symbol Element symbol
     * @param radius Atomic radius in Angstroms
     * @param mass Atomic mass in u
     */
    AtomicSpec(const std::string& symbol, double radius, double mass)
        : element_symbol(symbol), atomic_radius(radius), atomic_mass(mass) {}
};

} // namespace biomesh