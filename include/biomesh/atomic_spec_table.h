#pragma once

#include "atomic_spec.h"
#include <unordered_map>
#include <string>
#include <optional>

namespace biomesh {

/**
 * @brief Table managing atomic specifications for chemical elements
 */
class AtomicSpecTable {
private:
    std::unordered_map<std::string, AtomicSpec> specifications_;

public:
    /**
     * @brief Default constructor with standard biological elements
     */
    AtomicSpecTable();

    /**
     * @brief Add or update an atomic specification
     * @param spec Atomic specification to add
     */
    void addSpec(const AtomicSpec& spec);

    /**
     * @brief Get atomic specification by element symbol
     * @param element_symbol Element symbol to look up
     * @return Optional containing the specification if found
     */
    std::optional<AtomicSpec> getSpec(const std::string& element_symbol) const;

    /**
     * @brief Check if an element specification exists
     * @param element_symbol Element symbol to check
     * @return True if specification exists
     */
    bool hasSpec(const std::string& element_symbol) const;

    /**
     * @brief Remove a specification
     * @param element_symbol Element symbol to remove
     * @return True if specification was removed
     */
    bool removeSpec(const std::string& element_symbol);

    /**
     * @brief Clear all specifications
     */
    void clear();

    /**
     * @brief Get number of specifications
     * @return Number of specifications in table
     */
    size_t size() const;

private:
    /**
     * @brief Initialize with default biological elements
     */
    void initializeDefaults();
};

} // namespace biomesh