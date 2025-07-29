#include <gtest/gtest.h>
#include "Atom.h"
#include <sstream>
#include <cmath>

using namespace pdb;

class AtomTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create test atoms
        atom1_ = std::make_unique<Atom>(1, "CA", 1.0, 2.0, 3.0);
        atom2_ = std::make_unique<Atom>(2, "N", 4.0, 5.0, 6.0);
        atom3_ = std::make_unique<Atom>(1, "CA", 1.0, 2.0, 3.0); // Same as atom1
    }

    std::unique_ptr<Atom> atom1_;
    std::unique_ptr<Atom> atom2_;
    std::unique_ptr<Atom> atom3_;
};

TEST_F(AtomTest, ConstructorAndGetters) {
    EXPECT_EQ(atom1_->getId(), 1);
    EXPECT_EQ(atom1_->getType(), "CA");
    EXPECT_DOUBLE_EQ(atom1_->getX(), 1.0);
    EXPECT_DOUBLE_EQ(atom1_->getY(), 2.0);
    EXPECT_DOUBLE_EQ(atom1_->getZ(), 3.0);
}

TEST_F(AtomTest, DistanceFromOrigin) {
    // Distance from origin should be sqrt(1^2 + 2^2 + 3^2) = sqrt(14)
    double expectedDistance = std::sqrt(14.0);
    EXPECT_DOUBLE_EQ(atom1_->distanceFromOrigin(), expectedDistance);
}

TEST_F(AtomTest, DistanceFromOriginZero) {
    Atom origin(0, "O", 0.0, 0.0, 0.0);
    EXPECT_DOUBLE_EQ(origin.distanceFromOrigin(), 0.0);
}

TEST_F(AtomTest, DistanceBetweenAtoms) {
    // Distance between (1,2,3) and (4,5,6) should be sqrt(3^2 + 3^2 + 3^2) = sqrt(27)
    double expectedDistance = std::sqrt(27.0);
    EXPECT_DOUBLE_EQ(atom1_->distanceTo(*atom2_), expectedDistance);
    EXPECT_DOUBLE_EQ(atom2_->distanceTo(*atom1_), expectedDistance); // Should be symmetric
}

TEST_F(AtomTest, DistanceToSelf) {
    EXPECT_DOUBLE_EQ(atom1_->distanceTo(*atom1_), 0.0);
}

TEST_F(AtomTest, EqualityOperator) {
    EXPECT_TRUE(*atom1_ == *atom3_);  // Same values
    EXPECT_FALSE(*atom1_ == *atom2_); // Different values
}

TEST_F(AtomTest, EqualityWithFloatingPointTolerance) {
    // Test equality with very small differences (within epsilon)
    Atom closeAtom(1, "CA", 1.0000000001, 2.0, 3.0);
    EXPECT_TRUE(*atom1_ == closeAtom);
}

TEST_F(AtomTest, StreamOutput) {
    std::ostringstream oss;
    oss << *atom1_;
    std::string output = oss.str();
    
    // Check that output contains expected components
    EXPECT_NE(output.find("Atom{"), std::string::npos);
    EXPECT_NE(output.find("id=1"), std::string::npos);
    EXPECT_NE(output.find("type=CA"), std::string::npos);
    EXPECT_NE(output.find("1.000"), std::string::npos);
    EXPECT_NE(output.find("2.000"), std::string::npos);
    EXPECT_NE(output.find("3.000"), std::string::npos);
}

TEST_F(AtomTest, ConstructorWithMoveSemantics) {
    std::string atomType = "CB";
    Atom atom(5, std::move(atomType), 10.0, 20.0, 30.0);
    
    EXPECT_EQ(atom.getId(), 5);
    EXPECT_EQ(atom.getType(), "CB");
    EXPECT_DOUBLE_EQ(atom.getX(), 10.0);
    EXPECT_DOUBLE_EQ(atom.getY(), 20.0);
    EXPECT_DOUBLE_EQ(atom.getZ(), 30.0);
    
    // Original string should be empty after move
    EXPECT_TRUE(atomType.empty());
}

TEST_F(AtomTest, NegativeCoordinates) {
    Atom negativeAtom(10, "O", -1.5, -2.5, -3.5);
    
    EXPECT_DOUBLE_EQ(negativeAtom.getX(), -1.5);
    EXPECT_DOUBLE_EQ(negativeAtom.getY(), -2.5);
    EXPECT_DOUBLE_EQ(negativeAtom.getZ(), -3.5);
    
    double expectedDistance = std::sqrt(1.5*1.5 + 2.5*2.5 + 3.5*3.5);
    EXPECT_DOUBLE_EQ(negativeAtom.distanceFromOrigin(), expectedDistance);
}

TEST_F(AtomTest, LargeCoordinates) {
    Atom largeAtom(99, "FE", 1000.0, 2000.0, 3000.0);
    
    EXPECT_DOUBLE_EQ(largeAtom.getX(), 1000.0);
    EXPECT_DOUBLE_EQ(largeAtom.getY(), 2000.0);
    EXPECT_DOUBLE_EQ(largeAtom.getZ(), 3000.0);
}

TEST_F(AtomTest, EmptyAtomType) {
    // Test with empty atom type (should be allowed)
    Atom emptyTypeAtom(1, "", 1.0, 2.0, 3.0);
    EXPECT_EQ(emptyTypeAtom.getType(), "");
}

TEST_F(AtomTest, LongAtomType) {
    // Test with longer atom type
    Atom longTypeAtom(1, "CARBON", 1.0, 2.0, 3.0);
    EXPECT_EQ(longTypeAtom.getType(), "CARBON");
}