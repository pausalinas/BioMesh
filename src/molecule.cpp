#include "biomesh/molecule.h"
#include <limits>
#include <cmath>
#include <algorithm>
#include <stdexcept>

namespace biomesh {

Molecule::Molecule(const std::string& chain_id)
    : chain_id_(chain_id), bounding_box_cached_(false) {}

Molecule::Molecule(const Molecule& other)
    : chain_id_(other.chain_id_), atoms_(other.atoms_),
      bounding_box_cached_(other.bounding_box_cached_),
      bounding_box_(other.bounding_box_) {}

Molecule::Molecule(Molecule&& other) noexcept
    : chain_id_(std::move(other.chain_id_)), atoms_(std::move(other.atoms_)),
      bounding_box_cached_(other.bounding_box_cached_),
      bounding_box_(other.bounding_box_) {
    other.bounding_box_cached_ = false;
}

Molecule& Molecule::operator=(const Molecule& other) {
    if (this != &other) {
        chain_id_ = other.chain_id_;
        atoms_ = other.atoms_;
        bounding_box_cached_ = other.bounding_box_cached_;
        bounding_box_ = other.bounding_box_;
    }
    return *this;
}

Molecule& Molecule::operator=(Molecule&& other) noexcept {
    if (this != &other) {
        chain_id_ = std::move(other.chain_id_);
        atoms_ = std::move(other.atoms_);
        bounding_box_cached_ = other.bounding_box_cached_;
        bounding_box_ = other.bounding_box_;
        other.bounding_box_cached_ = false;
    }
    return *this;
}

void Molecule::addAtom(const Atom& atom) {
    atoms_.push_back(atom);
    invalidateBoundingBox();
}

void Molecule::addAtom(Atom&& atom) {
    atoms_.push_back(std::move(atom));
    invalidateBoundingBox();
}

const Atom& Molecule::getAtom(size_t index) const {
    if (index >= atoms_.size()) {
        throw std::out_of_range("Atom index out of range");
    }
    return atoms_[index];
}

Atom& Molecule::getAtom(size_t index) {
    if (index >= atoms_.size()) {
        throw std::out_of_range("Atom index out of range");
    }
    invalidateBoundingBox();
    return atoms_[index];
}

void Molecule::clear() {
    atoms_.clear();
    invalidateBoundingBox();
}

const BoundingBox& Molecule::getBoundingBox() const {
    if (!bounding_box_cached_) {
        calculateBoundingBox();
        bounding_box_cached_ = true;
    }
    return bounding_box_;
}

void Molecule::getCenter(double& center_x, double& center_y, double& center_z) const {
    const auto& bbox = getBoundingBox();
    bbox.getCenter(center_x, center_y, center_z);
}

double Molecule::getBoundingSphereRadius() const {
    const auto& bbox = getBoundingBox();
    return bbox.getBoundingSphereRadius();
}

bool Molecule::pointInBounds(double x, double y, double z) const {
    const auto& bbox = getBoundingBox();
    return bbox.contains(x, y, z);
}

void Molecule::invalidateBoundingBox() {
    bounding_box_cached_ = false;
}

void Molecule::calculateBoundingBox() const {
    if (atoms_.empty()) {
        bounding_box_ = BoundingBox{};
        return;
    }

    const auto& first_atom = atoms_[0];
    bounding_box_.min_x = bounding_box_.max_x = first_atom.x;
    bounding_box_.min_y = bounding_box_.max_y = first_atom.y;
    bounding_box_.min_z = bounding_box_.max_z = first_atom.z;

    for (size_t i = 1; i < atoms_.size(); ++i) {
        const auto& atom = atoms_[i];
        
        bounding_box_.min_x = std::min(bounding_box_.min_x, atom.x);
        bounding_box_.max_x = std::max(bounding_box_.max_x, atom.x);
        
        bounding_box_.min_y = std::min(bounding_box_.min_y, atom.y);
        bounding_box_.max_y = std::max(bounding_box_.max_y, atom.y);
        
        bounding_box_.min_z = std::min(bounding_box_.min_z, atom.z);
        bounding_box_.max_z = std::max(bounding_box_.max_z, atom.z);
    }
}

} // namespace biomesh