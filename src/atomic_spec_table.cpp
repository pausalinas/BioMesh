#include "biomesh/atomic_spec_table.h"

namespace biomesh {

AtomicSpecTable::AtomicSpecTable() {
    initializeDefaults();
}

void AtomicSpecTable::addSpec(const AtomicSpec& spec) {
    specifications_[spec.element_symbol] = spec;
}

std::optional<AtomicSpec> AtomicSpecTable::getSpec(const std::string& element_symbol) const {
    auto it = specifications_.find(element_symbol);
    if (it != specifications_.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool AtomicSpecTable::hasSpec(const std::string& element_symbol) const {
    return specifications_.find(element_symbol) != specifications_.end();
}

bool AtomicSpecTable::removeSpec(const std::string& element_symbol) {
    auto it = specifications_.find(element_symbol);
    if (it != specifications_.end()) {
        specifications_.erase(it);
        return true;
    }
    return false;
}

void AtomicSpecTable::clear() {
    specifications_.clear();
}

size_t AtomicSpecTable::size() const {
    return specifications_.size();
}

void AtomicSpecTable::initializeDefaults() {
    // Common biological elements with standard atomic radii and masses
    addSpec(AtomicSpec("C", 1.7, 12.01));   // Carbon
    addSpec(AtomicSpec("N", 1.55, 14.01));  // Nitrogen
    addSpec(AtomicSpec("O", 1.52, 16.00));  // Oxygen
    addSpec(AtomicSpec("S", 1.8, 32.07));   // Sulfur
    addSpec(AtomicSpec("P", 1.8, 30.97));   // Phosphorus
    addSpec(AtomicSpec("H", 1.2, 1.01));    // Hydrogen
    addSpec(AtomicSpec("CA", 1.97, 40.08)); // Calcium
    addSpec(AtomicSpec("MG", 1.73, 24.31)); // Magnesium
    addSpec(AtomicSpec("K", 2.75, 39.10));  // Potassium
    addSpec(AtomicSpec("NA", 2.27, 22.99)); // Sodium
    addSpec(AtomicSpec("CL", 1.75, 35.45)); // Chlorine
    addSpec(AtomicSpec("FE", 1.72, 55.85)); // Iron
    addSpec(AtomicSpec("ZN", 1.39, 65.38)); // Zinc
}

} // namespace biomesh