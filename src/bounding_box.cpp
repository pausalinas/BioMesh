#include "biomesh/bounding_box.h"
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

bool BoundingBox::contains(const BoundingBox& other) const {
    if (isEmpty() || other.isEmpty()) {
        return false;
    }
    
    return (other.minX_ >= minX_ && other.maxX_ <= maxX_ &&
            other.minY_ >= minY_ && other.maxY_ <= maxY_ &&
            other.minZ_ >= minZ_ && other.maxZ_ <= maxZ_);
}

bool BoundingBox::intersects(const BoundingBox& other) const {
    if (isEmpty() || other.isEmpty()) {
        return false;
    }
    
    // Check if boxes are separated in any dimension
    return !(other.maxX_ < minX_ || other.minX_ > maxX_ ||
             other.maxY_ < minY_ || other.minY_ > maxY_ ||
             other.maxZ_ < minZ_ || other.minZ_ > maxZ_);
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

std::array<BoundingBox, 8> BoundingBox::subdivide() const {
    std::array<BoundingBox, 8> octants;
    
    if (isEmpty()) {
        // Return array of empty boxes
        return octants;
    }
    
    // Calculate midpoints
    double midX = (minX_ + maxX_) * 0.5;
    double midY = (minY_ + maxY_) * 0.5;
    double midZ = (minZ_ + maxZ_) * 0.5;
    
    // Create 8 octants
    // Octant ordering: [X][Y][Z] where 0=lower half, 1=upper half
    octants[0] = BoundingBox(minX_, minY_, minZ_, midX, midY, midZ);     // [---]
    octants[1] = BoundingBox(minX_, minY_, midZ, midX, midY, maxZ_);     // [--+]
    octants[2] = BoundingBox(minX_, midY, minZ_, midX, maxY_, midZ);     // [-+-]
    octants[3] = BoundingBox(minX_, midY, midZ, midX, maxY_, maxZ_);     // [-++]
    octants[4] = BoundingBox(midX, minY_, minZ_, maxX_, midY, midZ);     // [+--]
    octants[5] = BoundingBox(midX, minY_, midZ, maxX_, midY, maxZ_);     // [+-+]
    octants[6] = BoundingBox(midX, midY, minZ_, maxX_, maxY_, midZ);     // [++-]
    octants[7] = BoundingBox(midX, midY, midZ, maxX_, maxY_, maxZ_);     // [+++]
    
    return octants;
}

void BoundingBox::initializeEmpty() {
    // Initialize to an invalid state where min > max to indicate empty
    minX_ = minY_ = minZ_ = std::numeric_limits<double>::max();
    maxX_ = maxY_ = maxZ_ = std::numeric_limits<double>::lowest();
}

} // namespace BioMesh