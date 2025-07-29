#include "Atom.h"
#include <cmath>
#include <iomanip>

namespace pdb {

Atom::Atom(int id, std::string type, double x, double y, double z)
    : id_(id), type_(std::move(type)), x_(x), y_(y), z_(z) {
    // Constructor uses initialization list for efficiency
}

double Atom::distanceFromOrigin() const noexcept {
    return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_);
}

double Atom::distanceTo(const Atom& other) const noexcept {
    const double dx = x_ - other.x_;
    const double dy = y_ - other.y_;
    const double dz = z_ - other.z_;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

bool Atom::operator==(const Atom& other) const noexcept {
    constexpr double epsilon = 1e-9;
    return id_ == other.id_ &&
           type_ == other.type_ &&
           std::abs(x_ - other.x_) < epsilon &&
           std::abs(y_ - other.y_) < epsilon &&
           std::abs(z_ - other.z_) < epsilon;
}

std::ostream& operator<<(std::ostream& os, const Atom& atom) {
    os << std::fixed << std::setprecision(3)
       << "Atom{id=" << atom.id_
       << ", type=" << atom.type_
       << ", coords=(" << atom.x_ << ", " << atom.y_ << ", " << atom.z_ << ")}";
    return os;
}

} // namespace pdb