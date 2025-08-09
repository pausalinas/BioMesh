#include "BoundingBox.h"
#include <algorithm>
#include <cmath>

namespace BioMesh {

BoundingBox::BoundingBox() {
    initializeEmpty();
}

BoundingBox::BoundingBox(double minX, double minY, double minZ, 
                         double maxX, double maxY, double maxZ)
    : minX_(minX), minY_(minY), minZ_(minZ),
      maxX_(maxX), maxY_(maxY), maxZ_(maxZ),
      isEmpty_(false) {
    // Ensure min <= max for all dimensions
    if (minX_ > maxX_) std::swap(minX_, maxX_);
    if (minY_ > maxY_) std::swap(minY_, maxY_);
    if (minZ_ > maxZ_) std::swap(minZ_, maxZ_);
}

void BoundingBox::addPoint(double x, double y, double z) {
    if (isEmpty_) {
        minX_ = maxX_ = x;
        minY_ = maxY_ = y;
        minZ_ = maxZ_ = z;
        isEmpty_ = false;
    } else {
        minX_ = std::min(minX_, x);
        minY_ = std::min(minY_, y);
        minZ_ = std::min(minZ_, z);
        maxX_ = std::max(maxX_, x);
        maxY_ = std::max(maxY_, y);
        maxZ_ = std::max(maxZ_, z);
    }
}

void BoundingBox::calculateFromAtoms(const std::vector<Atom>& atoms) {
    reset();
    
    for (const auto& atom : atoms) {
        addPoint(atom.getX(), atom.getY(), atom.getZ());
    }
}

double BoundingBox::getWidth() const {
    if (isEmpty_) return 0.0;
    return maxX_ - minX_;
}

double BoundingBox::getHeight() const {
    if (isEmpty_) return 0.0;
    return maxY_ - minY_;
}

double BoundingBox::getDepth() const {
    if (isEmpty_) return 0.0;
    return maxZ_ - minZ_;
}

void BoundingBox::getCenter(double& centerX, double& centerY, double& centerZ) const {
    if (isEmpty_) {
        centerX = centerY = centerZ = 0.0;
        return;
    }
    
    centerX = (minX_ + maxX_) * 0.5;
    centerY = (minY_ + maxY_) * 0.5;
    centerZ = (minZ_ + maxZ_) * 0.5;
}

bool BoundingBox::contains(double x, double y, double z) const {
    if (isEmpty_) return false;
    
    return (x >= minX_ && x <= maxX_ &&
            y >= minY_ && y <= maxY_ &&
            z >= minZ_ && z <= maxZ_);
}

bool BoundingBox::contains(const Atom& atom) const {
    return contains(atom.getX(), atom.getY(), atom.getZ());
}

void BoundingBox::reset() {
    initializeEmpty();
}

bool BoundingBox::isValid() const {
    if (isEmpty_) return true;
    
    return (minX_ <= maxX_ && minY_ <= maxY_ && minZ_ <= maxZ_);
}

void BoundingBox::expand(double margin) {
    if (isEmpty_) return;
    
    minX_ -= margin;
    minY_ -= margin;
    minZ_ -= margin;
    maxX_ += margin;
    maxY_ += margin;
    maxZ_ += margin;
}

double BoundingBox::getVolume() const {
    if (isEmpty_) return 0.0;
    
    return getWidth() * getHeight() * getDepth();
}

void BoundingBox::initializeEmpty() {
    minX_ = minY_ = minZ_ = std::numeric_limits<double>::max();
    maxX_ = maxY_ = maxZ_ = std::numeric_limits<double>::lowest();
    isEmpty_ = true;
}

} // namespace BioMesh