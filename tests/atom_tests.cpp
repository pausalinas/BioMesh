#include <gtest/gtest.h>
#include "Atom.h"
#include "AtomBuilder.h"
#include <vector>
#include <stdexcept>

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