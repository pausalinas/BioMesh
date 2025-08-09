#include <gtest/gtest.h>
#include "Atom.h"
#include "AtomBuilder.h"
#include "BoundingBox.h"
#include <vector>
#include <stdexcept>
#include <cmath>

using namespace BioMesh;

// Test fixtures for Atom class
class AtomTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup common test data
    }
};

// Test Atom constructors
TEST_F(AtomTest, DefaultConstructor) {
    Atom atom;
    EXPECT_EQ(atom.getX(), 0.0);
    EXPECT_EQ(atom.getY(), 0.0);
    EXPECT_EQ(atom.getZ(), 0.0);
    EXPECT_TRUE(atom.getChemicalElement().empty());
    EXPECT_EQ(atom.getAtomicRadius(), 0.0);
    EXPECT_EQ(atom.getAtomicMass(), 0.0);
}

TEST_F(AtomTest, CoordinatesAndElementConstructor) {
    Atom atom(1.5, 2.0, 3.5, "C");
    EXPECT_EQ(atom.getX(), 1.5);
    EXPECT_EQ(atom.getY(), 2.0);
    EXPECT_EQ(atom.getZ(), 3.5);
    EXPECT_EQ(atom.getChemicalElement(), "C");
    EXPECT_EQ(atom.getAtomicRadius(), 0.0);
    EXPECT_EQ(atom.getAtomicMass(), 0.0);
}

TEST_F(AtomTest, ElementOnlyConstructor) {
    Atom atom("N");
    EXPECT_EQ(atom.getX(), 0.0);
    EXPECT_EQ(atom.getY(), 0.0);
    EXPECT_EQ(atom.getZ(), 0.0);
    EXPECT_EQ(atom.getChemicalElement(), "N");
    EXPECT_EQ(atom.getAtomicRadius(), 0.0);
    EXPECT_EQ(atom.getAtomicMass(), 0.0);
}

TEST_F(AtomTest, ElementAndRadiusConstructor) {
    Atom atom("O", 1.52);
    EXPECT_EQ(atom.getX(), 0.0);
    EXPECT_EQ(atom.getY(), 0.0);
    EXPECT_EQ(atom.getZ(), 0.0);
    EXPECT_EQ(atom.getChemicalElement(), "O");
    EXPECT_EQ(atom.getAtomicRadius(), 1.52);
    EXPECT_EQ(atom.getAtomicMass(), 0.0);
}

TEST_F(AtomTest, ElementRadiusAndMassConstructor) {
    Atom atom("P", 1.80, 30.974);
    EXPECT_EQ(atom.getX(), 0.0);
    EXPECT_EQ(atom.getY(), 0.0);
    EXPECT_EQ(atom.getZ(), 0.0);
    EXPECT_EQ(atom.getChemicalElement(), "P");
    EXPECT_EQ(atom.getAtomicRadius(), 1.80);
    EXPECT_EQ(atom.getAtomicMass(), 30.974);
}

// Test setters
TEST_F(AtomTest, SettersWork) {
    Atom atom;
    atom.setCoordinates(10.0, 20.0, 30.0);
    atom.setChemicalElement("S");
    atom.setAtomicRadius(1.80);
    atom.setAtomicMass(32.06);

    EXPECT_EQ(atom.getX(), 10.0);
    EXPECT_EQ(atom.getY(), 20.0);
    EXPECT_EQ(atom.getZ(), 30.0);
    EXPECT_EQ(atom.getChemicalElement(), "S");
    EXPECT_EQ(atom.getAtomicRadius(), 1.80);
    EXPECT_EQ(atom.getAtomicMass(), 32.06);
}

TEST_F(AtomTest, IndividualCoordinateSetters) {
    Atom atom;
    atom.setX(5.0);
    atom.setY(15.0);
    atom.setZ(25.0);

    EXPECT_EQ(atom.getX(), 5.0);
    EXPECT_EQ(atom.getY(), 15.0);
    EXPECT_EQ(atom.getZ(), 25.0);
}

// Test fixtures for AtomBuilder class
class AtomBuilderTest : public ::testing::Test {
protected:
    void SetUp() override {
        builder = std::make_unique<AtomBuilder>();
    }

    std::unique_ptr<AtomBuilder> builder;
};

// Test AtomBuilder functionality
TEST_F(AtomBuilderTest, HasCommonElements) {
    EXPECT_TRUE(builder->hasElement("H"));
    EXPECT_TRUE(builder->hasElement("C"));
    EXPECT_TRUE(builder->hasElement("N"));
    EXPECT_TRUE(builder->hasElement("O"));
    EXPECT_TRUE(builder->hasElement("P"));
    EXPECT_TRUE(builder->hasElement("S"));
    EXPECT_FALSE(builder->hasElement("Xx")); // Non-existent element
}

TEST_F(AtomBuilderTest, GetAtomicSpecReturnsCorrectValues) {
    const auto& carbonSpec = builder->getAtomicSpec("C");
    EXPECT_EQ(carbonSpec.elementSymbol, "C");
    EXPECT_EQ(carbonSpec.radius, 1.70);
    EXPECT_EQ(carbonSpec.mass, 12.011);

    const auto& oxygenSpec = builder->getAtomicSpec("O");
    EXPECT_EQ(oxygenSpec.elementSymbol, "O");
    EXPECT_EQ(oxygenSpec.radius, 1.52);
    EXPECT_EQ(oxygenSpec.mass, 15.999);
}

TEST_F(AtomBuilderTest, GetAtomicSpecThrowsForUnknownElement) {
    EXPECT_THROW(builder->getAtomicSpec("Xx"), std::runtime_error);
}

TEST_F(AtomBuilderTest, AddAtomicSpecWorks) {
    builder->addAtomicSpec("Test", 2.0, 100.0);
    EXPECT_TRUE(builder->hasElement("Test"));
    
    const auto& testSpec = builder->getAtomicSpec("Test");
    EXPECT_EQ(testSpec.elementSymbol, "Test");
    EXPECT_EQ(testSpec.radius, 2.0);
    EXPECT_EQ(testSpec.mass, 100.0);
}

TEST_F(AtomBuilderTest, BuildAtomsWithKnownElements) {
    // Create parsed atoms with only coordinates and elements
    std::vector<Atom> parsedAtoms;
    parsedAtoms.emplace_back(1.0, 2.0, 3.0, "C");
    parsedAtoms.emplace_back(4.0, 5.0, 6.0, "N");
    parsedAtoms.emplace_back(7.0, 8.0, 9.0, "O");

    // Build enhanced atoms
    auto enhancedAtoms = builder->buildAtoms(parsedAtoms);

    ASSERT_EQ(enhancedAtoms.size(), 3);

    // Check carbon atom
    const auto& carbon = enhancedAtoms[0];
    EXPECT_EQ(carbon.getX(), 1.0);
    EXPECT_EQ(carbon.getY(), 2.0);
    EXPECT_EQ(carbon.getZ(), 3.0);
    EXPECT_EQ(carbon.getChemicalElement(), "C");
    EXPECT_EQ(carbon.getAtomicRadius(), 1.70);
    EXPECT_EQ(carbon.getAtomicMass(), 12.011);

    // Check nitrogen atom
    const auto& nitrogen = enhancedAtoms[1];
    EXPECT_EQ(nitrogen.getX(), 4.0);
    EXPECT_EQ(nitrogen.getY(), 5.0);
    EXPECT_EQ(nitrogen.getZ(), 6.0);
    EXPECT_EQ(nitrogen.getChemicalElement(), "N");
    EXPECT_EQ(nitrogen.getAtomicRadius(), 1.55);
    EXPECT_EQ(nitrogen.getAtomicMass(), 14.007);

    // Check oxygen atom
    const auto& oxygen = enhancedAtoms[2];
    EXPECT_EQ(oxygen.getX(), 7.0);
    EXPECT_EQ(oxygen.getY(), 8.0);
    EXPECT_EQ(oxygen.getZ(), 9.0);
    EXPECT_EQ(oxygen.getChemicalElement(), "O");
    EXPECT_EQ(oxygen.getAtomicRadius(), 1.52);
    EXPECT_EQ(oxygen.getAtomicMass(), 15.999);
}

TEST_F(AtomBuilderTest, BuildAtomsThrowsForUnknownElement) {
    std::vector<Atom> parsedAtoms;
    parsedAtoms.emplace_back(1.0, 2.0, 3.0, "UnknownElement");

    EXPECT_THROW(builder->buildAtoms(parsedAtoms), std::runtime_error);
}

TEST_F(AtomBuilderTest, BuildAtomsWithEmptyVector) {
    std::vector<Atom> parsedAtoms;
    auto enhancedAtoms = builder->buildAtoms(parsedAtoms);
    EXPECT_TRUE(enhancedAtoms.empty());
}

// Integration test
TEST_F(AtomBuilderTest, IntegrationTestWithMixedElements) {
    std::vector<Atom> parsedAtoms;
    parsedAtoms.emplace_back(0.0, 0.0, 0.0, "H");   // Hydrogen
    parsedAtoms.emplace_back(1.0, 0.0, 0.0, "C");   // Carbon
    parsedAtoms.emplace_back(2.0, 0.0, 0.0, "N");   // Nitrogen
    parsedAtoms.emplace_back(3.0, 0.0, 0.0, "O");   // Oxygen
    parsedAtoms.emplace_back(4.0, 0.0, 0.0, "P");   // Phosphorus
    parsedAtoms.emplace_back(5.0, 0.0, 0.0, "S");   // Sulfur

    auto enhancedAtoms = builder->buildAtoms(parsedAtoms);
    ASSERT_EQ(enhancedAtoms.size(), 6);

    // Verify all atoms have correct properties assigned
    EXPECT_EQ(enhancedAtoms[0].getAtomicRadius(), 1.20);  // H
    EXPECT_EQ(enhancedAtoms[1].getAtomicRadius(), 1.70);  // C
    EXPECT_EQ(enhancedAtoms[2].getAtomicRadius(), 1.55);  // N
    EXPECT_EQ(enhancedAtoms[3].getAtomicRadius(), 1.52);  // O
    EXPECT_EQ(enhancedAtoms[4].getAtomicRadius(), 1.80);  // P
    EXPECT_EQ(enhancedAtoms[5].getAtomicRadius(), 1.80);  // S

    EXPECT_EQ(enhancedAtoms[0].getAtomicMass(), 1.008);   // H
    EXPECT_EQ(enhancedAtoms[1].getAtomicMass(), 12.011);  // C
    EXPECT_EQ(enhancedAtoms[2].getAtomicMass(), 14.007);  // N
    EXPECT_EQ(enhancedAtoms[3].getAtomicMass(), 15.999);  // O
    EXPECT_EQ(enhancedAtoms[4].getAtomicMass(), 30.974);  // P
    EXPECT_EQ(enhancedAtoms[5].getAtomicMass(), 32.06);   // S
}

// Test fixtures for BoundingBox class
class BoundingBoxTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup common test data
    }
};

// Test BoundingBox constructors
TEST_F(BoundingBoxTest, DefaultConstructor) {
    BoundingBox box;
    EXPECT_TRUE(box.isEmpty());
    EXPECT_TRUE(box.isValid());
    EXPECT_EQ(box.getWidth(), 0.0);
    EXPECT_EQ(box.getHeight(), 0.0);
    EXPECT_EQ(box.getDepth(), 0.0);
    EXPECT_EQ(box.getVolume(), 0.0);
}

TEST_F(BoundingBoxTest, ExplicitBoundsConstructor) {
    BoundingBox box(-1.0, -2.0, -3.0, 4.0, 5.0, 6.0);
    EXPECT_FALSE(box.isEmpty());
    EXPECT_TRUE(box.isValid());
    
    EXPECT_EQ(box.getMinX(), -1.0);
    EXPECT_EQ(box.getMinY(), -2.0);
    EXPECT_EQ(box.getMinZ(), -3.0);
    EXPECT_EQ(box.getMaxX(), 4.0);
    EXPECT_EQ(box.getMaxY(), 5.0);
    EXPECT_EQ(box.getMaxZ(), 6.0);
    
    EXPECT_EQ(box.getWidth(), 5.0);
    EXPECT_EQ(box.getHeight(), 7.0);
    EXPECT_EQ(box.getDepth(), 9.0);
    EXPECT_EQ(box.getVolume(), 315.0);
}

TEST_F(BoundingBoxTest, ExplicitBoundsConstructorInvalidBounds) {
    // Test that invalid bounds throw an exception
    EXPECT_THROW(BoundingBox(1.0, 0.0, 0.0, 0.0, 1.0, 1.0), std::invalid_argument);
    EXPECT_THROW(BoundingBox(0.0, 1.0, 0.0, 1.0, 0.0, 1.0), std::invalid_argument);
    EXPECT_THROW(BoundingBox(0.0, 0.0, 1.0, 1.0, 1.0, 0.0), std::invalid_argument);
}

// Test addPoint functionality
TEST_F(BoundingBoxTest, AddSinglePoint) {
    BoundingBox box;
    box.addPoint(1.0, 2.0, 3.0);
    
    EXPECT_FALSE(box.isEmpty());
    EXPECT_TRUE(box.isValid());
    
    EXPECT_EQ(box.getMinX(), 1.0);
    EXPECT_EQ(box.getMinY(), 2.0);
    EXPECT_EQ(box.getMinZ(), 3.0);
    EXPECT_EQ(box.getMaxX(), 1.0);
    EXPECT_EQ(box.getMaxY(), 2.0);
    EXPECT_EQ(box.getMaxZ(), 3.0);
    
    EXPECT_EQ(box.getWidth(), 0.0);
    EXPECT_EQ(box.getHeight(), 0.0);
    EXPECT_EQ(box.getDepth(), 0.0);
    EXPECT_EQ(box.getVolume(), 0.0);
}

TEST_F(BoundingBoxTest, AddMultiplePoints) {
    BoundingBox box;
    box.addPoint(1.0, 2.0, 3.0);
    box.addPoint(-1.0, 4.0, 2.0);
    box.addPoint(3.0, 1.0, 5.0);
    
    EXPECT_FALSE(box.isEmpty());
    EXPECT_TRUE(box.isValid());
    
    EXPECT_EQ(box.getMinX(), -1.0);
    EXPECT_EQ(box.getMinY(), 1.0);
    EXPECT_EQ(box.getMinZ(), 2.0);
    EXPECT_EQ(box.getMaxX(), 3.0);
    EXPECT_EQ(box.getMaxY(), 4.0);
    EXPECT_EQ(box.getMaxZ(), 5.0);
    
    EXPECT_EQ(box.getWidth(), 4.0);
    EXPECT_EQ(box.getHeight(), 3.0);
    EXPECT_EQ(box.getDepth(), 3.0);
    EXPECT_EQ(box.getVolume(), 36.0);
}

// Test calculateFromAtoms
TEST_F(BoundingBoxTest, CalculateFromAtomsEmpty) {
    BoundingBox box;
    std::vector<Atom> atoms;
    
    box.calculateFromAtoms(atoms);
    EXPECT_TRUE(box.isEmpty());
}

TEST_F(BoundingBoxTest, CalculateFromAtomsSingle) {
    BoundingBox box;
    std::vector<Atom> atoms;
    atoms.emplace_back(1.0, 2.0, 3.0, "C");
    
    box.calculateFromAtoms(atoms);
    
    EXPECT_FALSE(box.isEmpty());
    EXPECT_EQ(box.getMinX(), 1.0);
    EXPECT_EQ(box.getMinY(), 2.0);
    EXPECT_EQ(box.getMinZ(), 3.0);
    EXPECT_EQ(box.getMaxX(), 1.0);
    EXPECT_EQ(box.getMaxY(), 2.0);
    EXPECT_EQ(box.getMaxZ(), 3.0);
}

TEST_F(BoundingBoxTest, CalculateFromAtomsMultiple) {
    BoundingBox box;
    std::vector<Atom> atoms;
    atoms.emplace_back(1.0, 2.0, 3.0, "C");
    atoms.emplace_back(-1.0, 4.0, 2.0, "N");
    atoms.emplace_back(3.0, 1.0, 5.0, "O");
    
    box.calculateFromAtoms(atoms);
    
    EXPECT_FALSE(box.isEmpty());
    EXPECT_EQ(box.getMinX(), -1.0);
    EXPECT_EQ(box.getMinY(), 1.0);
    EXPECT_EQ(box.getMinZ(), 2.0);
    EXPECT_EQ(box.getMaxX(), 3.0);
    EXPECT_EQ(box.getMaxY(), 4.0);
    EXPECT_EQ(box.getMaxZ(), 5.0);
}

// Test center calculation
TEST_F(BoundingBoxTest, GetCenterEmpty) {
    BoundingBox box;
    double centerX, centerY, centerZ;
    
    box.getCenter(centerX, centerY, centerZ);
    
    // Empty box should have NaN center
    EXPECT_TRUE(std::isnan(centerX));
    EXPECT_TRUE(std::isnan(centerY));
    EXPECT_TRUE(std::isnan(centerZ));
}

TEST_F(BoundingBoxTest, GetCenterNonEmpty) {
    BoundingBox box(-2.0, -4.0, -6.0, 4.0, 8.0, 12.0);
    double centerX, centerY, centerZ;
    
    box.getCenter(centerX, centerY, centerZ);
    
    EXPECT_EQ(centerX, 1.0);
    EXPECT_EQ(centerY, 2.0);
    EXPECT_EQ(centerZ, 3.0);
}

// Test containment
TEST_F(BoundingBoxTest, ContainsPointEmpty) {
    BoundingBox box;
    EXPECT_FALSE(box.contains(0.0, 0.0, 0.0));
    EXPECT_FALSE(box.contains(1.0, 2.0, 3.0));
}

TEST_F(BoundingBoxTest, ContainsPointNonEmpty) {
    BoundingBox box(-1.0, -1.0, -1.0, 1.0, 1.0, 1.0);
    
    // Points inside
    EXPECT_TRUE(box.contains(0.0, 0.0, 0.0));
    EXPECT_TRUE(box.contains(0.5, -0.5, 0.8));
    
    // Points on boundary (inclusive)
    EXPECT_TRUE(box.contains(-1.0, -1.0, -1.0));
    EXPECT_TRUE(box.contains(1.0, 1.0, 1.0));
    EXPECT_TRUE(box.contains(0.0, 1.0, -1.0));
    
    // Points outside
    EXPECT_FALSE(box.contains(1.1, 0.0, 0.0));
    EXPECT_FALSE(box.contains(0.0, 1.1, 0.0));
    EXPECT_FALSE(box.contains(0.0, 0.0, 1.1));
    EXPECT_FALSE(box.contains(-1.1, 0.0, 0.0));
}

TEST_F(BoundingBoxTest, ContainsAtom) {
    BoundingBox box(-1.0, -1.0, -1.0, 1.0, 1.0, 1.0);
    
    Atom atomInside(0.0, 0.0, 0.0, "C");
    Atom atomOutside(2.0, 0.0, 0.0, "N");
    Atom atomOnBoundary(1.0, 1.0, 1.0, "O");
    
    EXPECT_TRUE(box.contains(atomInside));
    EXPECT_FALSE(box.contains(atomOutside));
    EXPECT_TRUE(box.contains(atomOnBoundary));
}

// Test state management
TEST_F(BoundingBoxTest, ResetToEmpty) {
    BoundingBox box(-1.0, -1.0, -1.0, 1.0, 1.0, 1.0);
    EXPECT_FALSE(box.isEmpty());
    
    box.reset();
    EXPECT_TRUE(box.isEmpty());
    EXPECT_TRUE(box.isValid());
}

TEST_F(BoundingBoxTest, IsValidEmpty) {
    BoundingBox box;
    EXPECT_TRUE(box.isValid());
}

TEST_F(BoundingBoxTest, IsValidNonEmpty) {
    BoundingBox box(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
    EXPECT_TRUE(box.isValid());
}

// Test expansion
TEST_F(BoundingBoxTest, ExpandEmpty) {
    BoundingBox box;
    box.expand(1.0);
    
    // Expansion should have no effect on empty box
    EXPECT_TRUE(box.isEmpty());
}

TEST_F(BoundingBoxTest, ExpandNonEmpty) {
    BoundingBox box(-1.0, -1.0, -1.0, 1.0, 1.0, 1.0);
    double originalVolume = box.getVolume();
    
    box.expand(1.0);
    
    EXPECT_EQ(box.getMinX(), -2.0);
    EXPECT_EQ(box.getMinY(), -2.0);
    EXPECT_EQ(box.getMinZ(), -2.0);
    EXPECT_EQ(box.getMaxX(), 2.0);
    EXPECT_EQ(box.getMaxY(), 2.0);
    EXPECT_EQ(box.getMaxZ(), 2.0);
    
    EXPECT_EQ(box.getWidth(), 4.0);
    EXPECT_EQ(box.getHeight(), 4.0);
    EXPECT_EQ(box.getDepth(), 4.0);
    EXPECT_EQ(box.getVolume(), 64.0);
    
    EXPECT_GT(box.getVolume(), originalVolume);
}

TEST_F(BoundingBoxTest, ExpandNegativeMargin) {
    BoundingBox box(-2.0, -2.0, -2.0, 2.0, 2.0, 2.0);
    
    box.expand(-1.0);  // Shrink by 1.0
    
    EXPECT_EQ(box.getMinX(), -1.0);
    EXPECT_EQ(box.getMinY(), -1.0);
    EXPECT_EQ(box.getMinZ(), -1.0);
    EXPECT_EQ(box.getMaxX(), 1.0);
    EXPECT_EQ(box.getMaxY(), 1.0);
    EXPECT_EQ(box.getMaxZ(), 1.0);
    
    EXPECT_EQ(box.getWidth(), 2.0);
    EXPECT_EQ(box.getHeight(), 2.0);
    EXPECT_EQ(box.getDepth(), 2.0);
    EXPECT_EQ(box.getVolume(), 8.0);
}

// Integration test with AtomBuilder
TEST_F(BoundingBoxTest, IntegrationWithAtomBuilder) {
    // Create parsed atoms
    std::vector<Atom> parsedAtoms;
    parsedAtoms.emplace_back(0.0, 0.0, 0.0, "C");
    parsedAtoms.emplace_back(1.0, 1.0, 1.0, "N");
    parsedAtoms.emplace_back(-1.0, -1.0, -1.0, "O");
    
    // Enhance with AtomBuilder
    AtomBuilder builder;
    auto enhancedAtoms = builder.buildAtoms(parsedAtoms);
    
    // Calculate bounding box
    BoundingBox box;
    box.calculateFromAtoms(enhancedAtoms);
    
    EXPECT_FALSE(box.isEmpty());
    EXPECT_EQ(box.getMinX(), -1.0);
    EXPECT_EQ(box.getMinY(), -1.0);
    EXPECT_EQ(box.getMinZ(), -1.0);
    EXPECT_EQ(box.getMaxX(), 1.0);
    EXPECT_EQ(box.getMaxY(), 1.0);
    EXPECT_EQ(box.getMaxZ(), 1.0);
    
    // Test that all enhanced atoms are contained
    for (const auto& atom : enhancedAtoms) {
        EXPECT_TRUE(box.contains(atom));
    }
    
    // Test center calculation
    double centerX, centerY, centerZ;
    box.getCenter(centerX, centerY, centerZ);
    EXPECT_EQ(centerX, 0.0);
    EXPECT_EQ(centerY, 0.0);
    EXPECT_EQ(centerZ, 0.0);
    
    // Test volume
    EXPECT_EQ(box.getVolume(), 8.0);  // 2x2x2 cube
}