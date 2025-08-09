#include "AtomBuilder.h"
#include <stdexcept>

namespace BioMesh {

AtomBuilder::AtomBuilder() {
    initializeAtomicSpecs();
}

std::vector<Atom> AtomBuilder::buildAtoms(const std::vector<Atom>& parsedAtoms) const {
    std::vector<Atom> enhancedAtoms;
    enhancedAtoms.reserve(parsedAtoms.size());

    for (const auto& parsedAtom : parsedAtoms) {
        const std::string& element = parsedAtom.getChemicalElement();
        
        // Check if element exists in specification table
        auto it = atomicSpecs_.find(element);
        if (it == atomicSpecs_.end()) {
            throw std::runtime_error("Element '" + element + "' not found in atomic specification table");
        }

        // Create enhanced atom with all properties
        Atom enhancedAtom = parsedAtom;  // Copy coordinates and element
        enhancedAtom.setAtomicRadius(it->second.radius);
        enhancedAtom.setAtomicMass(it->second.mass);
        
        enhancedAtoms.push_back(enhancedAtom);
    }

    return enhancedAtoms;
}

void AtomBuilder::addAtomicSpec(const std::string& element, double radius, double mass) {
    atomicSpecs_[element] = AtomicSpec(element, radius, mass);
}

bool AtomBuilder::hasElement(const std::string& element) const {
    return atomicSpecs_.find(element) != atomicSpecs_.end();
}

const AtomicSpec& AtomBuilder::getAtomicSpec(const std::string& element) const {
    auto it = atomicSpecs_.find(element);
    if (it == atomicSpecs_.end()) {
        throw std::runtime_error("Element '" + element + "' not found in atomic specification table");
    }
    return it->second;
}

void AtomBuilder::initializeAtomicSpecs() {
    // Common biological elements with accurate atomic properties
    // Values from NIST and standard chemistry references
    
    // Hydrogen
    addAtomicSpec("H", 1.20, 1.008);
    
    // Carbon
    addAtomicSpec("C", 1.70, 12.011);
    
    // Nitrogen  
    addAtomicSpec("N", 1.55, 14.007);
    
    // Oxygen
    addAtomicSpec("O", 1.52, 15.999);
    
    // Phosphorus
    addAtomicSpec("P", 1.80, 30.974);
    
    // Sulfur
    addAtomicSpec("S", 1.80, 32.06);
    
    // Additional common elements
    addAtomicSpec("Na", 2.27, 22.990);  // Sodium
    addAtomicSpec("Mg", 1.73, 24.305);  // Magnesium
    addAtomicSpec("Cl", 1.75, 35.45);   // Chlorine
    addAtomicSpec("K", 2.75, 39.098);   // Potassium
    addAtomicSpec("Ca", 2.31, 40.078);  // Calcium
    addAtomicSpec("Fe", 2.04, 55.845);  // Iron
    addAtomicSpec("Zn", 2.01, 65.38);   // Zinc
    addAtomicSpec("Cu", 1.96, 63.546);  // Copper
    addAtomicSpec("Mn", 2.05, 54.938);  // Manganese
    addAtomicSpec("Co", 1.92, 58.933);  // Cobalt
    addAtomicSpec("Ni", 1.84, 58.693);  // Nickel
    addAtomicSpec("Mo", 2.17, 95.95);   // Molybdenum
    addAtomicSpec("Se", 1.90, 78.971);  // Selenium
    addAtomicSpec("I", 1.98, 126.90);   // Iodine
}

} // namespace BioMesh