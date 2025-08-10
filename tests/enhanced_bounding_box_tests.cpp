#include <gtest/gtest.h>
#include "biomesh/bounding_box.h"
#include <vector>
#include <stdexcept>
#include <cmath>

using namespace BioMesh;

// Test fixtures for enhanced BoundingBox functionality
class EnhancedBoundingBoxTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup common test data
        unitBox = BoundingBox(-1.0, -1.0, -1.0, 1.0, 1.0, 1.0);
        largeBox = BoundingBox(-2.0, -2.0, -2.0, 2.0, 2.0, 2.0);
        smallBox = BoundingBox(-0.5, -0.5, -0.5, 0.5, 0.5, 0.5);
        offsetBox = BoundingBox(0.5, 0.5, 0.5, 1.5, 1.5, 1.5);
        separatedBox = BoundingBox(2.0, 2.0, 2.0, 3.0, 3.0, 3.0);
    }
    
    BoundingBox unitBox;     // [-1, 1] in all dimensions
    BoundingBox largeBox;    // [-2, 2] in all dimensions
    BoundingBox smallBox;    // [-0.5, 0.5] in all dimensions
    BoundingBox offsetBox;   // [0.5, 1.5] in all dimensions (overlaps unitBox)
    BoundingBox separatedBox; // [2, 3] in all dimensions (separated from unitBox)
};

// Test BoundingBox contains(BoundingBox) functionality
TEST_F(EnhancedBoundingBoxTest, ContainsBoundingBoxBasic) {
    // Large box should contain smaller boxes
    EXPECT_TRUE(largeBox.contains(unitBox));
    EXPECT_TRUE(largeBox.contains(smallBox));
    EXPECT_TRUE(unitBox.contains(smallBox));
    
    // Small box should not contain larger boxes
    EXPECT_FALSE(smallBox.contains(unitBox));
    EXPECT_FALSE(smallBox.contains(largeBox));
    EXPECT_FALSE(unitBox.contains(largeBox));
}

TEST_F(EnhancedBoundingBoxTest, ContainsBoundingBoxEdgeCases) {
    // Box should contain itself
    EXPECT_TRUE(unitBox.contains(unitBox));
    EXPECT_TRUE(largeBox.contains(largeBox));
    
    // Empty boxes
    BoundingBox emptyBox;
    EXPECT_FALSE(unitBox.contains(emptyBox));
    EXPECT_FALSE(emptyBox.contains(unitBox));
    EXPECT_FALSE(emptyBox.contains(emptyBox));
    
    // Overlapping but not contained
    EXPECT_FALSE(unitBox.contains(offsetBox));
    EXPECT_FALSE(offsetBox.contains(unitBox));
    
    // Separated boxes
    EXPECT_FALSE(unitBox.contains(separatedBox));
    EXPECT_FALSE(separatedBox.contains(unitBox));
}

TEST_F(EnhancedBoundingBoxTest, ContainsBoundingBoxBoundaryConditions) {
    // Box that exactly matches bounds should be contained
    BoundingBox exactMatch(-1.0, -1.0, -1.0, 1.0, 1.0, 1.0);
    EXPECT_TRUE(unitBox.contains(exactMatch));
    
    // Box that extends slightly outside should not be contained
    BoundingBox slightlyLarger(-1.1, -1.0, -1.0, 1.0, 1.0, 1.0);
    EXPECT_FALSE(unitBox.contains(slightlyLarger));
}

// Test BoundingBox intersects functionality
TEST_F(EnhancedBoundingBoxTest, IntersectsBasic) {
    // Overlapping boxes should intersect
    EXPECT_TRUE(unitBox.intersects(offsetBox));
    EXPECT_TRUE(offsetBox.intersects(unitBox));
    
    // Contained boxes should intersect
    EXPECT_TRUE(unitBox.intersects(smallBox));
    EXPECT_TRUE(smallBox.intersects(unitBox));
    EXPECT_TRUE(largeBox.intersects(unitBox));
    EXPECT_TRUE(unitBox.intersects(largeBox));
    
    // Separated boxes should not intersect
    EXPECT_FALSE(unitBox.intersects(separatedBox));
    EXPECT_FALSE(separatedBox.intersects(unitBox));
}

TEST_F(EnhancedBoundingBoxTest, IntersectsEdgeCases) {
    // Box should intersect with itself
    EXPECT_TRUE(unitBox.intersects(unitBox));
    
    // Empty boxes should not intersect
    BoundingBox emptyBox;
    EXPECT_FALSE(unitBox.intersects(emptyBox));
    EXPECT_FALSE(emptyBox.intersects(unitBox));
    EXPECT_FALSE(emptyBox.intersects(emptyBox));
}

TEST_F(EnhancedBoundingBoxTest, IntersectsBoundaryTouching) {
    // Boxes that just touch at a face should intersect
    BoundingBox touchingBox(1.0, -1.0, -1.0, 2.0, 1.0, 1.0);
    EXPECT_TRUE(unitBox.intersects(touchingBox));
    
    // Boxes that just touch at an edge should intersect
    BoundingBox edgeTouchingBox(1.0, 1.0, -1.0, 2.0, 2.0, 1.0);
    EXPECT_TRUE(unitBox.intersects(edgeTouchingBox));
    
    // Boxes that just touch at a corner should intersect
    BoundingBox cornerTouchingBox(1.0, 1.0, 1.0, 2.0, 2.0, 2.0);
    EXPECT_TRUE(unitBox.intersects(cornerTouchingBox));
}

// Test subdivide functionality
TEST_F(EnhancedBoundingBoxTest, SubdivideBasic) {
    auto octants = unitBox.subdivide();
    
    // Should have 8 octants
    EXPECT_EQ(octants.size(), 8);
    
    // All octants should be valid and non-empty
    for (const auto& octant : octants) {
        EXPECT_TRUE(octant.isValid());
        EXPECT_FALSE(octant.isEmpty());
    }
    
    // Check dimensions of each octant
    for (const auto& octant : octants) {
        EXPECT_EQ(octant.getWidth(), 1.0);  // Half of original width (2.0)
        EXPECT_EQ(octant.getHeight(), 1.0); // Half of original height (2.0)
        EXPECT_EQ(octant.getDepth(), 1.0);  // Half of original depth (2.0)
        EXPECT_EQ(octant.getVolume(), 1.0); // (1.0)^3
    }
}

TEST_F(EnhancedBoundingBoxTest, SubdivideOctantPositions) {
    auto octants = unitBox.subdivide();
    
    // Test specific octant positions
    // Octant 0: [---] should be [-1, 0] in all dimensions
    EXPECT_EQ(octants[0].getMinX(), -1.0);
    EXPECT_EQ(octants[0].getMaxX(), 0.0);
    EXPECT_EQ(octants[0].getMinY(), -1.0);
    EXPECT_EQ(octants[0].getMaxY(), 0.0);
    EXPECT_EQ(octants[0].getMinZ(), -1.0);
    EXPECT_EQ(octants[0].getMaxZ(), 0.0);
    
    // Octant 7: [+++] should be [0, 1] in all dimensions
    EXPECT_EQ(octants[7].getMinX(), 0.0);
    EXPECT_EQ(octants[7].getMaxX(), 1.0);
    EXPECT_EQ(octants[7].getMinY(), 0.0);
    EXPECT_EQ(octants[7].getMaxY(), 1.0);
    EXPECT_EQ(octants[7].getMinZ(), 0.0);
    EXPECT_EQ(octants[7].getMaxZ(), 1.0);
}

TEST_F(EnhancedBoundingBoxTest, SubdivideContainment) {
    auto octants = unitBox.subdivide();
    
    // Original box should contain all octants
    for (const auto& octant : octants) {
        EXPECT_TRUE(unitBox.contains(octant));
    }
    
    // Octants should not contain each other (they're the same size and adjacent)
    // but they may contain themselves
    for (size_t i = 0; i < octants.size(); ++i) {
        for (size_t j = 0; j < octants.size(); ++j) {
            if (i != j) {
                EXPECT_FALSE(octants[i].contains(octants[j]));
            } else {
                EXPECT_TRUE(octants[i].contains(octants[j])); // Box contains itself
            }
        }
    }
}

TEST_F(EnhancedBoundingBoxTest, SubdivideIntersection) {
    auto octants = unitBox.subdivide();
    
    // Adjacent octants should intersect (they share faces)
    // Octant 0 [---] and Octant 1 [--+] share a face in Z
    EXPECT_TRUE(octants[0].intersects(octants[1]));
    
    // Octant 0 [---] and Octant 2 [-+-] share a face in Y
    EXPECT_TRUE(octants[0].intersects(octants[2]));
    
    // Octant 0 [---] and Octant 4 [+--] share a face in X
    EXPECT_TRUE(octants[0].intersects(octants[4]));
    
    // Opposite octants should only touch at a corner
    EXPECT_TRUE(octants[0].intersects(octants[7])); // Should touch at (0,0,0)
}

TEST_F(EnhancedBoundingBoxTest, SubdivideEmpty) {
    BoundingBox emptyBox;
    auto octants = emptyBox.subdivide();
    
    // Should have 8 octants, all empty
    EXPECT_EQ(octants.size(), 8);
    for (const auto& octant : octants) {
        EXPECT_TRUE(octant.isEmpty());
        EXPECT_TRUE(octant.isValid());
    }
}

TEST_F(EnhancedBoundingBoxTest, SubdivideNonCubic) {
    // Test subdivision of a non-cubic box
    BoundingBox rectBox(0.0, 0.0, 0.0, 4.0, 2.0, 6.0);
    auto octants = rectBox.subdivide();
    
    // All octants should be valid
    for (const auto& octant : octants) {
        EXPECT_TRUE(octant.isValid());
        EXPECT_FALSE(octant.isEmpty());
    }
    
    // Check dimensions - each should be half of original
    EXPECT_EQ(octants[0].getWidth(), 2.0);  // Half of 4.0
    EXPECT_EQ(octants[0].getHeight(), 1.0); // Half of 2.0
    EXPECT_EQ(octants[0].getDepth(), 3.0);  // Half of 6.0
    
    // Total volume should be preserved
    double originalVolume = rectBox.getVolume();
    double totalOctantVolume = 0.0;
    for (const auto& octant : octants) {
        totalOctantVolume += octant.getVolume();
    }
    EXPECT_DOUBLE_EQ(originalVolume, totalOctantVolume);
}

// Integration test combining all new functionality
TEST_F(EnhancedBoundingBoxTest, IntegrationMeshGenerationScenario) {
    // Simulate a mesh generation scenario
    BoundingBox molecularSpace(-10.0, -10.0, -10.0, 10.0, 10.0, 10.0);
    
    // First level subdivision
    auto firstLevel = molecularSpace.subdivide();
    EXPECT_EQ(firstLevel.size(), 8);
    
    // Each first-level octant should have volume 1/8 of original
    double originalVolume = molecularSpace.getVolume();
    for (const auto& octant : firstLevel) {
        EXPECT_DOUBLE_EQ(octant.getVolume(), originalVolume / 8.0);
    }
    
    // Second level subdivision of first octant
    auto secondLevel = firstLevel[0].subdivide();
    EXPECT_EQ(secondLevel.size(), 8);
    
    // Each second-level octant should have volume 1/64 of original
    for (const auto& octant : secondLevel) {
        EXPECT_DOUBLE_EQ(octant.getVolume(), originalVolume / 64.0);
    }
    
    // Test containment hierarchy
    for (const auto& secondLevelOctant : secondLevel) {
        EXPECT_TRUE(firstLevel[0].contains(secondLevelOctant));
        EXPECT_TRUE(molecularSpace.contains(secondLevelOctant));
    }
    
    // Test intersection with a query region
    BoundingBox queryRegion(-5.0, -5.0, -5.0, 5.0, 5.0, 5.0);
    int intersectingOctants = 0;
    for (const auto& octant : firstLevel) {
        if (octant.intersects(queryRegion)) {
            intersectingOctants++;
        }
    }
    
    // Should have multiple intersecting octants
    EXPECT_GT(intersectingOctants, 0);
    EXPECT_LE(intersectingOctants, 8);
}