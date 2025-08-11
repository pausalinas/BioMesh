#pragma once

#include "Atom.h"
#include <vector>
#include <limits>
#include <array>

namespace BioMesh {

/**
 * @brief 3D bounding box for spatial calculations with atomic and molecular data
 * 
 * The BoundingBox class provides comprehensive 3D spatial calculations for collections
 * of atoms and points. It maintains minimum and maximum coordinates and provides
 * methods for spatial analysis, containment testing, and geometric calculations.
 * 
 * The bounding box can be in one of three states:
 * - Empty: No points have been added (minX > maxX indicates empty state)
 * - Single point: minX == maxX, minY == maxY, minZ == maxZ
 * - Normal: minX <= maxX, minY <= maxY, minZ <= maxZ with positive dimensions
 * 
 * Enhanced with octree support for mesh generation operations.
 */
class BoundingBox {
public:
    /**
     * @brief Default constructor creates an empty bounding box
     */
    BoundingBox();

    /**
     * @brief Constructor with explicit bounds
     * @param minX Minimum X coordinate
     * @param minY Minimum Y coordinate
     * @param minZ Minimum Z coordinate
     * @param maxX Maximum X coordinate
     * @param maxY Maximum Y coordinate
     * @param maxZ Maximum Z coordinate
     * @throws std::invalid_argument if any min coordinate is greater than corresponding max
     */
    BoundingBox(double minX, double minY, double minZ, double maxX, double maxY, double maxZ);

    /**
     * @brief Copy constructor
     */
    BoundingBox(const BoundingBox& other) = default;

    /**
     * @brief Move constructor
     */
    BoundingBox(BoundingBox&& other) noexcept = default;

    /**
     * @brief Copy assignment operator
     */
    BoundingBox& operator=(const BoundingBox& other) = default;

    /**
     * @brief Move assignment operator
     */
    BoundingBox& operator=(BoundingBox&& other) noexcept = default;

    /**
     * @brief Destructor
     */
    ~BoundingBox() = default;

    // Core functionality

    /**
     * @brief Add a single point to expand the bounding box
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     */
    void addPoint(double x, double y, double z);

    /**
     * @brief Calculate bounds from a collection of atoms
     * @param atoms Vector of atoms to calculate bounds from
     * @note This method resets the bounding box and calculates new bounds from the atoms
     */
    void calculateFromAtoms(const std::vector<Atom>& atoms);

    // Getters for bounds
    
    /**
     * @brief Get minimum X coordinate
     * @return Minimum X coordinate (NaN if empty)
     */
    double getMinX() const { return minX_; }

    /**
     * @brief Get minimum Y coordinate
     * @return Minimum Y coordinate (NaN if empty)
     */
    double getMinY() const { return minY_; }

    /**
     * @brief Get minimum Z coordinate
     * @return Minimum Z coordinate (NaN if empty)
     */
    double getMinZ() const { return minZ_; }

    /**
     * @brief Get maximum X coordinate
     * @return Maximum X coordinate (NaN if empty)
     */
    double getMaxX() const { return maxX_; }

    /**
     * @brief Get maximum Y coordinate
     * @return Maximum Y coordinate (NaN if empty)
     */
    double getMaxY() const { return maxY_; }

    /**
     * @brief Get maximum Z coordinate
     * @return Maximum Z coordinate (NaN if empty)
     */
    double getMaxZ() const { return maxZ_; }

    // Dimension methods

    /**
     * @brief Get width (X dimension) of the bounding box
     * @return Width of the box (0.0 if empty)
     */
    double getWidth() const;

    /**
     * @brief Get height (Y dimension) of the bounding box
     * @return Height of the box (0.0 if empty)
     */
    double getHeight() const;

    /**
     * @brief Get depth (Z dimension) of the bounding box
     * @return Depth of the box (0.0 if empty)
     */
    double getDepth() const;

    /**
     * @brief Get volume of the bounding box
     * @return Volume of the box (0.0 if empty)
     */
    double getVolume() const;

    /**
     * @brief Get center point of the bounding box
     * @param centerX Reference to store center X coordinate
     * @param centerY Reference to store center Y coordinate
     * @param centerZ Reference to store center Z coordinate
     * @note If the box is empty, center coordinates will be NaN
     */
    void getCenter(double& centerX, double& centerY, double& centerZ) const;

    // Containment tests

    /**
     * @brief Test if a point is contained within the bounding box
     * @param x X coordinate of the point
     * @param y Y coordinate of the point
     * @param z Z coordinate of the point
     * @return true if point is inside the box (inclusive), false otherwise
     * @note Returns false for empty bounding boxes
     */
    bool contains(double x, double y, double z) const;

    /**
     * @brief Test if an atom is contained within the bounding box
     * @param atom The atom to test for containment
     * @return true if atom's coordinates are inside the box (inclusive), false otherwise
     * @note This method only checks the atom's center coordinates, not its radius
     * @note Returns false for empty bounding boxes
     */
    bool contains(const Atom& atom) const;

    /**
     * @brief Test if another bounding box is completely contained within this box
     * @param other The bounding box to test for containment
     * @return true if other box is completely inside this box, false otherwise
     * @note Returns false if either box is empty
     */
    bool contains(const BoundingBox& other) const;

    /**
     * @brief Test if another bounding box intersects with this box
     * @param other The bounding box to test for intersection
     * @return true if the boxes intersect or touch, false otherwise
     * @note Returns false if either box is empty
     */
    bool intersects(const BoundingBox& other) const;

    // State management

    /**
     * @brief Reset the bounding box to empty state
     */
    void reset();

    /**
     * @brief Check if the bounding box is empty
     * @return true if no points have been added, false otherwise
     */
    bool isEmpty() const;

    /**
     * @brief Validate that min <= max for all dimensions
     * @return true if the bounding box is in a valid state, false otherwise
     * @note Empty boxes are considered valid
     */
    bool isValid() const;

    /**
     * @brief Expand the bounding box by a margin in all directions
     * @param margin Amount to expand in each direction (positive value)
     * @note If the box is empty, this method has no effect
     * @note Negative margins will shrink the box, but won't make it invalid
     */
    void expand(double margin);

    // Octree support for mesh generation

    /**
     * @brief Subdivide the bounding box into 8 octants for octree operations
     * @return Array of 8 child bounding boxes representing the octants
     * @note Returns empty boxes if this box is empty
     * @note Octants are ordered as: [---], [--+], [-+-], [-++], [+--], [+-+], [++-], [+++]
     *       where - means lower half and + means upper half in X, Y, Z respectively
     */
    std::array<BoundingBox, 8> subdivide() const;

private:
    double minX_;  ///< Minimum X coordinate
    double minY_;  ///< Minimum Y coordinate
    double minZ_;  ///< Minimum Z coordinate
    double maxX_;  ///< Maximum X coordinate
    double maxY_;  ///< Maximum Y coordinate
    double maxZ_;  ///< Maximum Z coordinate

    /**
     * @brief Initialize to empty state
     */
    void initializeEmpty();
};

} // namespace BioMesh