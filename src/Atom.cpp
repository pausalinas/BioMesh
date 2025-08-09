#include "Atom.h"

namespace BioMesh {

Atom::Atom(double x, double y, double z, const std::string& chemicalElement)
    : coordinates_{x, y, z}, chemicalElement_(chemicalElement), atomicRadius_(0.0), atomicMass_(0.0) {
}

Atom::Atom(const std::string& chemicalElement)
    : coordinates_{0.0, 0.0, 0.0}, chemicalElement_(chemicalElement), atomicRadius_(0.0), atomicMass_(0.0) {
}

Atom::Atom(const std::string& chemicalElement, double atomicRadius)
    : coordinates_{0.0, 0.0, 0.0}, chemicalElement_(chemicalElement), atomicRadius_(atomicRadius), atomicMass_(0.0) {
}

Atom::Atom(const std::string& chemicalElement, double atomicRadius, double atomicMass)
    : coordinates_{0.0, 0.0, 0.0}, chemicalElement_(chemicalElement), atomicRadius_(atomicRadius), atomicMass_(atomicMass) {
}

void Atom::setCoordinates(double x, double y, double z) {
    coordinates_[0] = x;
    coordinates_[1] = y;
    coordinates_[2] = z;
}

} // namespace BioMesh