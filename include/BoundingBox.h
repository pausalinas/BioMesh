#pragma once

#include <vector>
#include <limits>
#include "Atom.h"

namespace BioMesh {

/**
 * @brief A standalone 3D bounding box class for spatial calculations
 * 
 * This class provides a reusable bounding box implementation that can work
 * with any 3D coordinate data. It follows RAII principles and provides
 * efficient methods for spatial operations commonly needed in mesh generation.
 */
class BoundingBox {
public:
    /**
     * @brief Default constructor - creates an empty bounding box
     */
    BoundingBox();

    /**
     * @brief Constructor with initial bounds
     * 
     * @param minX Minimum X coordinate
     * @param minY Minimum Y coordinate
     * @param minZ Minimum Z coordinate
     * @param maxX Maximum X coordinate
     * @param maxY Maximum Y coordinate
     * @param maxZ Maximum Z coordinate
     */
    BoundingBox(double minX, double minY, double minZ, 
                double maxX, double maxY, double maxZ);

    /**
     * @brief Copy constructor
     */
    BoundingBox(const BoundingBox& other) = default;

    /**
     * @brief Assignment operator
     */
    BoundingBox& operator=(const BoundingBox& other) = default;

    /**
     * @brief Move constructor
     */
    BoundingBox(BoundingBox&& other) = default;

    /**
     * @brief Move assignment operator
     */
    BoundingBox& operator=(BoundingBox&& other) = default;

    /**
     * @brief Destructor
     */
    ~BoundingBox() = default;

    /**
     * @brief Add a single point to expand the bounding box
     * 
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     */
    void addPoint(double x, double y, double z);

    /**
     * @brief Calculate bounding box from a collection of atoms
     * 
     * @param atoms Vector of atoms
     */
    void calculateFromAtoms(const std::vector<Atom>& atoms);

    /**
     * @brief Get minimum X coordinate
     * 
     * @return double Minimum X coordinate
     */
    double getMinX() const { return minX_; }

    /**
     * @brief Get minimum Y coordinate
     * 
     * @return double Minimum Y coordinate
     */
    double getMinY() const { return minY_; }

    /**
     * @brief Get minimum Z coordinate
     * 
     * @return double Minimum Z coordinate
     */
    double getMinZ() const { return minZ_; }

    /**
     * @brief Get maximum X coordinate
     * 
     * @return double Maximum X coordinate
     */
    double getMaxX() const { return maxX_; }

    /**
     * @brief Get maximum Y coordinate
     * 
     * @return double Maximum Y coordinate
     */
    double getMaxY() const { return maxY_; }

    /**
     * @brief Get maximum Z coordinate
     * 
     * @return double Maximum Z coordinate
     */
    double getMaxZ() const { return maxZ_; }

    /**
     * @brief Get the width (X dimension) of the bounding box
     * 
     * @return double Width of the bounding box
     */
    double getWidth() const;

    /**
     * @brief Get the height (Y dimension) of the bounding box
     * 
     * @return double Height of the bounding box
     */
    double getHeight() const;

    /**
     * @brief Get the depth (Z dimension) of the bounding box
     * 
     * @return double Depth of the bounding box
     */
    double getDepth() const;

    /**
     * @brief Get the center point of the bounding box
     * 
     * @param centerX Output parameter for center X coordinate
     * @param centerY Output parameter for center Y coordinate
     * @param centerZ Output parameter for center Z coordinate
     */
    void getCenter(double& centerX, double& centerY, double& centerZ) const;

    /**
     * @brief Check if a point is contained within the bounding box
     * 
     * @param x X coordinate
     * @param y Y coordinate
     * @param z Z coordinate
     * @return true if point is within bounds, false otherwise
     */
    bool contains(double x, double y, double z) const;

    /**
     * @brief Check if an atom is contained within the bounding box
     * 
     * @param atom Atom to check
     * @return true if atom is within bounds, false otherwise
     */
    bool contains(const Atom& atom) const;

    /**
     * @brief Reset/clear the bounding box to empty state
     */
    void reset();

    /**
     * @brief Check if the bounding box is empty (no points added)
     * 
     * @return true if empty, false otherwise
     */
    bool isEmpty() const { return isEmpty_; }

    /**
     * @brief Check if the bounding box is valid (min <= max for all dimensions)
     * 
     * @return true if valid, false otherwise
     */
    bool isValid() const;

    /**
     * @brief Expand the bounding box by a given margin in all directions
     * 
     * @param margin Margin to add in all directions
     */
    void expand(double margin);

    /**
     * @brief Get the volume of the bounding box
     * 
     * @return double Volume of the bounding box
     */
    double getVolume() const;

private:
    double minX_; ///< Minimum X coordinate
    double minY_; ///< Minimum Y coordinate
    double minZ_; ///< Minimum Z coordinate
    double maxX_; ///< Maximum X coordinate
    double maxY_; ///< Maximum Y coordinate
    double maxZ_; ///< Maximum Z coordinate
    bool isEmpty_; ///< Flag indicating if bounding box is empty

    /**
     * @brief Initialize to empty state
     */
    void initializeEmpty();
};

} // namespace BioMesh