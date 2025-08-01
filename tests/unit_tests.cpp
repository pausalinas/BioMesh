#include "PDBReader.h"
#include <iostream>
#include <cassert>
#include <fstream>
#include <filesystem>

class TestRunner {
public:
    TestRunner() : tests_passed_(0), tests_failed_(0) {}
    
    void runTest(const std::string& testName, bool (*testFunc)()) {
        std::cout << "Running " << testName << "... ";
        if (testFunc()) {
            std::cout << "PASSED" << std::endl;
            tests_passed_++;
        } else {
            std::cout << "FAILED" << std::endl;
            tests_failed_++;
        }
    }
    
    void printSummary() {
        std::cout << std::endl << "=== Test Summary ===" << std::endl;
        std::cout << "Tests passed: " << tests_passed_ << std::endl;
        std::cout << "Tests failed: " << tests_failed_ << std::endl;
        std::cout << "Total tests: " << (tests_passed_ + tests_failed_) << std::endl;
        
        if (tests_failed_ == 0) {
            std::cout << "All tests PASSED!" << std::endl;
        } else {
            std::cout << "Some tests FAILED!" << std::endl;
        }
    }
    
    bool allTestsPassed() const { return tests_failed_ == 0; }
    
private:
    int tests_passed_;
    int tests_failed_;
};

// Test functions
bool testBasicPDBReading() {
    BioMesh::PDBReader reader;
    
    // Create a temporary test file
    std::ofstream tempFile("temp_test.pdb");
    tempFile << "ATOM      1  N   ALA A   1      20.154  -6.351   1.610  1.00 10.00           N\n";
    tempFile << "ATOM      2  CA  ALA A   1      19.030  -6.621   2.498  1.00 10.00           C\n";
    tempFile.close();
    
    bool result = reader.readFile("temp_test.pdb");
    std::filesystem::remove("temp_test.pdb");
    
    return result && reader.getAtomCount() == 2 && !reader.hasErrors();
}

bool testNonExistentFile() {
    BioMesh::PDBReader reader;
    bool result = reader.readFile("nonexistent.pdb");
    return !result && reader.hasErrors();
}

bool testEmptyFile() {
    BioMesh::PDBReader reader;
    
    std::ofstream tempFile("empty_test.pdb");
    tempFile.close();
    
    bool result = reader.readFile("empty_test.pdb");
    std::filesystem::remove("empty_test.pdb");
    
    return result && reader.getAtomCount() == 0 && !reader.getWarnings().empty();
}

bool testMalformedAtomLines() {
    BioMesh::PDBReader reader;
    
    std::ofstream tempFile("malformed_test.pdb");
    tempFile << "ATOM      1  N   ALA A   1      20.154  -6.351   1.610  1.00 10.00           N\n";
    tempFile << "ATOM      2  CA  ALA A   1\n"; // Too short
    tempFile << "ATOM      3  C   ALA A   1      19.290  -7.853   3.339  1.00 10.00           C\n";
    tempFile.close();
    
    bool result = reader.readFile("malformed_test.pdb");
    std::filesystem::remove("malformed_test.pdb");
    
    return result && reader.getAtomCount() == 2 && reader.hasErrors();
}

bool testHETATMWarning() {
    BioMesh::PDBReader reader;
    
    std::ofstream tempFile("hetatm_test.pdb");
    tempFile << "ATOM      1  N   ALA A   1      20.154  -6.351   1.610  1.00 10.00           N\n";
    tempFile << "HETATM    2  C1  LIG A 101      10.123  -2.456   3.789  1.00 20.00           C\n";
    tempFile << "ATOM      3  CA  ALA A   1      19.030  -6.621   2.498  1.00 10.00           C\n";
    tempFile.close();
    
    bool result = reader.readFile("hetatm_test.pdb");
    std::filesystem::remove("hetatm_test.pdb");
    
    // Should have warnings about HETATM records but still succeed
    return result && reader.getAtomCount() == 2 && !reader.getWarnings().empty();
}

bool testCoordinateParsing() {
    BioMesh::PDBReader reader;
    
    std::ofstream tempFile("coord_test.pdb");
    tempFile << "ATOM      1  CA  ALA A   1     -12.345  67.890-100.123  1.00 25.50           C\n";
    tempFile.close();
    
    bool result = reader.readFile("coord_test.pdb");
    std::filesystem::remove("coord_test.pdb");
    
    if (!result || reader.getAtomCount() != 1) return false;
    
    const auto& atoms = reader.getAtoms();
    const auto& atom = atoms[0];
    
    return abs(atom.x - (-12.345)) < 0.001 &&
           abs(atom.y - 67.890) < 0.001 &&
           abs(atom.z - (-100.123)) < 0.001 &&
           abs(atom.occupancy - 1.00) < 0.001 &&
           abs(atom.tempFactor - 25.50) < 0.001;
}

bool testAtomProperties() {
    BioMesh::PDBReader reader;
    
    std::ofstream tempFile("props_test.pdb");
    tempFile << "ATOM    123  CB  VAL B  45      10.000  20.000  30.000  0.75 15.25           C\n";
    tempFile.close();
    
    bool result = reader.readFile("props_test.pdb");
    std::filesystem::remove("props_test.pdb");
    
    if (!result || reader.getAtomCount() != 1) return false;
    
    const auto& atoms = reader.getAtoms();
    const auto& atom = atoms[0];
    
    return atom.serial == 123 &&
           atom.name == "CB" &&
           atom.resName == "VAL" &&
           atom.chainID == 'B' &&
           atom.resSeq == 45 &&
           atom.element == "C";
}

bool testMultipleChains() {
    BioMesh::PDBReader reader;
    
    std::ofstream tempFile("chains_test.pdb");
    tempFile << "ATOM      1  CA  ALA A   1      10.000  10.000  10.000  1.00 10.00           C\n";
    tempFile << "ATOM      2  CA  ALA A   2      11.000  11.000  11.000  1.00 10.00           C\n";
    tempFile << "ATOM      3  CA  GLY B   1      20.000  20.000  20.000  1.00 10.00           C\n";
    tempFile << "ATOM      4  CA  GLY B   2      21.000  21.000  21.000  1.00 10.00           C\n";
    tempFile.close();
    
    bool result = reader.readFile("chains_test.pdb");
    std::filesystem::remove("chains_test.pdb");
    
    if (!result || reader.getAtomCount() != 4) return false;
    
    return reader.getAtomCountByChain('A') == 2 &&
           reader.getAtomCountByChain('B') == 2 &&
           reader.getAtomCountByChain('C') == 0;
}

bool testCoordinateRanges() {
    BioMesh::PDBReader reader;
    
    std::ofstream tempFile("ranges_test.pdb");
    tempFile << "ATOM      1  CA  ALA A   1      -5.000  -10.000  -15.000  1.00 10.00           C\n";
    tempFile << "ATOM      2  CA  ALA A   2       5.000   10.000   15.000  1.00 10.00           C\n";
    tempFile.close();
    
    bool result = reader.readFile("ranges_test.pdb");
    std::filesystem::remove("ranges_test.pdb");
    
    if (!result) return false;
    
    std::string ranges = reader.getCoordinateRanges();
    // Should contain range information
    return ranges.find("X:") != std::string::npos &&
           ranges.find("Y:") != std::string::npos &&
           ranges.find("Z:") != std::string::npos;
}

bool testStatisticsPrinting() {
    BioMesh::PDBReader reader;
    
    std::ofstream tempFile("stats_test.pdb");
    tempFile << "ATOM      1  CA  ALA A   1      10.000  10.000  10.000  1.00 10.00           C\n";
    tempFile << "ATOM      2  CA  GLY B   1      20.000  20.000  20.000  1.00 10.00           C\n";
    tempFile.close();
    
    bool result = reader.readFile("stats_test.pdb");
    std::filesystem::remove("stats_test.pdb");
    
    if (!result) return false;
    
    // This is mainly a smoke test - we just check it doesn't crash
    reader.printStatistics();
    return true;
}

// Test with existing test files if they exist
bool testExistingTestFiles() {
    std::string testDataDir = "tests/test_data/";
    bool allPassed = true;
    
    std::vector<std::string> testFiles = {
        "protein_small.pdb",
        "mixed_structure.pdb",
        "malformed.pdb"
    };
    
    for (const auto& filename : testFiles) {
        std::string fullPath = testDataDir + filename;
        if (std::filesystem::exists(fullPath)) {
            BioMesh::PDBReader reader;
            if (filename == "malformed.pdb") {
                // Malformed file should still read successfully but have errors
                reader.readFile(fullPath);
                if (!reader.hasErrors()) {
                    std::cout << "Expected errors for " << filename << " but got none. ";
                    allPassed = false;
                }
            } else if (filename == "mixed_structure.pdb") {
                // Mixed structure should have warnings about HETATM
                bool result = reader.readFile(fullPath);
                if (!result || reader.getWarnings().empty()) {
                    std::cout << "Expected warnings for " << filename << ". ";
                    allPassed = false;
                }
            } else {
                // Regular files should read successfully
                bool result = reader.readFile(fullPath);
                if (!result || reader.getAtomCount() == 0) {
                    std::cout << "Failed to read " << filename << ". ";
                    allPassed = false;
                }
            }
        }
    }
    
    return allPassed;
}

int main() {
    std::cout << "=== BioMesh PDBReader Unit Tests ===" << std::endl << std::endl;
    
    TestRunner runner;
    
    // Basic functionality tests
    runner.runTest("Basic PDB Reading", testBasicPDBReading);
    runner.runTest("Non-existent File", testNonExistentFile);
    runner.runTest("Empty File", testEmptyFile);
    runner.runTest("Malformed ATOM Lines", testMalformedAtomLines);
    runner.runTest("HETATM Warning", testHETATMWarning);
    
    // Data parsing tests
    runner.runTest("Coordinate Parsing", testCoordinateParsing);
    runner.runTest("Atom Properties", testAtomProperties);
    runner.runTest("Multiple Chains", testMultipleChains);
    
    // Analysis tests
    runner.runTest("Coordinate Ranges", testCoordinateRanges);
    runner.runTest("Statistics Printing", testStatisticsPrinting);
    
    // Test with existing files
    runner.runTest("Existing Test Files", testExistingTestFiles);
    
    runner.printSummary();
    
    return runner.allTestsPassed() ? 0 : 1;
}