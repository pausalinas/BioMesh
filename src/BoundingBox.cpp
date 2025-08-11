#include "BoundingBox.h"
#include <stdexcept>
#include <cmath>
#include <algorithm>

namespace BioMesh {

BoundingBox::BoundingBox() {
    initializeEmpty();
}

BoundingBox::BoundingBox(double minX, double minY, double minZ, double maxX, double maxY, double maxZ)
    : minX_(minX), minY_(minY), minZ_(minZ), maxX_(maxX), maxY_(maxY), maxZ_(maxZ) {
    
    // Validate that min <= max for all dimensions
    if (minX > maxX || minY > maxY || minZ > maxZ) {
        throw std::invalid_argument("Invalid bounding box: min coordinates must be <= max coordinates");
    }
}

void BoundingBox::addPoint(double x, double y, double z) {
    if (isEmpty()) {
        // First point sets both min and max
        minX_ = maxX_ = x;
        minY_ = maxY_ = y;
        minZ_ = maxZ_ = z;
    } else {
        // Expand bounds to include new point
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
    if (isEmpty()) {
        return 0.0;
    }
    return maxX_ - minX_;
}

double BoundingBox::getHeight() const {
    if (isEmpty()) {
        return 0.0;
    }
    return maxY_ - minY_;
}

double BoundingBox::getDepth() const {
    if (isEmpty()) {
        return 0.0;
    }
    return maxZ_ - minZ_;
}

double BoundingBox::getVolume() const {
    if (isEmpty()) {
        return 0.0;
    }
    return getWidth() * getHeight() * getDepth();
}

void BoundingBox::getCenter(double& centerX, double& centerY, double& centerZ) const {
    if (isEmpty()) {
        centerX = centerY = centerZ = std::numeric_limits<double>::quiet_NaN();
    } else {
        centerX = (minX_ + maxX_) * 0.5;
        centerY = (minY_ + maxY_) * 0.5;
        centerZ = (minZ_ + maxZ_) * 0.5;
    }
}

bool BoundingBox::contains(double x, double y, double z) const {
    if (isEmpty()) {
        return false;
    }
    
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

bool BoundingBox::isEmpty() const {
    // Use the fact that we initialize empty boxes with min > max
    return minX_ > maxX_;
}

bool BoundingBox::isValid() const {
    // Empty boxes are valid, and non-empty boxes must have min <= max
    return isEmpty() || (minX_ <= maxX_ && minY_ <= maxY_ && minZ_ <= maxZ_);
}

void BoundingBox::expand(double margin) {
    if (isEmpty()) {
        // No effect on empty boxes
        return;
    }
    
    minX_ -= margin;
    minY_ -= margin;
    minZ_ -= margin;
    maxX_ += margin;
    maxY_ += margin;
    maxZ_ += margin;
}

void BoundingBox::initializeEmpty() {
    // Initialize to an invalid state where min > max to indicate empty
    minX_ = minY_ = minZ_ = std::numeric_limits<double>::max();
    maxX_ = maxY_ = maxZ_ = std::numeric_limits<double>::lowest();
}

} // namespace BioMesh