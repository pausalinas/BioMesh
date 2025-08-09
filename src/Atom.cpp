#include "Atom.h"

namespace BioMesh {

Atom::Atom(double x, double y, double z, const std::string& element, const std::string& name)
    : x_(x), y_(y), z_(z), element_(element), name_(name) {
}

void Atom::setCoordinates(double x, double y, double z) {
    x_ = x;
    y_ = y;
    z_ = z;
}

void Atom::setElement(const std::string& element) {
    element_ = element;
}

void Atom::setName(const std::string& name) {
    name_ = name;
}

} // namespace BioMesh