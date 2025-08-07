#include "biomesh/atom_builder.h"
#include <cctype>
#include <algorithm>

namespace biomesh {

AtomBuilder::AtomBuilder(std::shared_ptr<AtomicSpecTable> spec_table)
    : spec_table_(spec_table) {
    if (!spec_table_) {
        spec_table_ = std::make_shared<AtomicSpecTable>();
    }
}

AtomBuilder& AtomBuilder::setSerial(int serial) {
    atom_.serial_number = serial;
    return *this;
}

AtomBuilder& AtomBuilder::setAtomName(const std::string& name) {
    atom_.atom_name = name;
    return *this;
}

AtomBuilder& AtomBuilder::setResidueName(const std::string& name) {
    atom_.residue_name = name;
    return *this;
}

AtomBuilder& AtomBuilder::setChainId(const std::string& chain) {
    atom_.chain_id = chain;
    return *this;
}

AtomBuilder& AtomBuilder::setResidueNumber(int number) {
    atom_.residue_number = number;
    return *this;
}

AtomBuilder& AtomBuilder::setCoordinates(double x, double y, double z) {
    atom_.x = x;
    atom_.y = y;
    atom_.z = z;
    return *this;
}

AtomBuilder& AtomBuilder::setChemicalElement(const std::string& element) {
    atom_.chemical_element = element;
    return *this;
}

AtomBuilder& AtomBuilder::setAtomicRadius(double radius) {
    atom_.atomic_radius = radius;
    return *this;
}

AtomBuilder& AtomBuilder::setAtomicMass(double mass) {
    atom_.atomic_mass = mass;
    return *this;
}

AtomBuilder& AtomBuilder::autoDetectElement() {
    if (!atom_.atom_name.empty()) {
        std::string element = extractElementFromName(atom_.atom_name);
        if (!element.empty()) {
            atom_.chemical_element = element;
            applySpecification(element);
        }
    }
    return *this;
}

Atom AtomBuilder::build() {
    // If chemical element is not set but atom name is available, try auto-detection
    if (atom_.chemical_element.empty() && !atom_.atom_name.empty()) {
        autoDetectElement();
    }
    
    return atom_;
}

AtomBuilder& AtomBuilder::reset() {
    atom_ = Atom{};
    return *this;
}

std::string AtomBuilder::extractElementFromName(const std::string& atom_name) const {
    if (atom_name.empty()) {
        return "";
    }

    // Common PDB atom name patterns:
    // - Single element: "C", "N", "O", "S", "P", "H"
    // - With numbers: "C1", "N1", "O1", etc.
    // - Greek letters: "CA" (C-alpha), "CB" (C-beta), etc.
    // - Special cases: "FE", "ZN", "MG", "CA" (calcium vs C-alpha)

    std::string name = atom_name;
    
    // Remove leading/trailing whitespace
    name.erase(0, name.find_first_not_of(" \t"));
    name.erase(name.find_last_not_of(" \t") + 1);

    if (name.empty()) {
        return "";
    }

    // Special two-letter elements
    if (name.length() >= 2) {
        std::string two_letter = name.substr(0, 2);
        std::transform(two_letter.begin(), two_letter.end(), two_letter.begin(), ::toupper);
        
        if (two_letter == "FE" || two_letter == "ZN" || two_letter == "MG" || 
            two_letter == "CL" || two_letter == "NA" || two_letter == "CA") {
            // Check if this is really calcium or C-alpha
            if (two_letter == "CA") {
                // Heuristic: if atom name is exactly "CA", it's likely C-alpha (carbon)
                // If it's in a non-protein context, it might be calcium
                // For now, default to carbon for "CA"
                return "C";
            }
            return two_letter;
        }
    }

    // Single letter elements
    char first_char = std::toupper(name[0]);
    switch (first_char) {
        case 'C': return "C";
        case 'N': return "N";
        case 'O': return "O";
        case 'S': return "S";
        case 'P': return "P";
        case 'H': return "H";
        case 'K': return "K";
        default: return "";
    }
}

void AtomBuilder::applySpecification(const std::string& element) {
    if (spec_table_) {
        auto spec = spec_table_->getSpec(element);
        if (spec.has_value()) {
            // Only apply if not already set
            if (atom_.atomic_radius == 0.0) {
                atom_.atomic_radius = spec->atomic_radius;
            }
            if (atom_.atomic_mass == 0.0) {
                atom_.atomic_mass = spec->atomic_mass;
            }
        }
    }
}

} // namespace biomesh