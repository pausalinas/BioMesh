#include "PDBReader.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <filesystem>
#include <iomanip>

class IntegrationTestRunner {
public:
    IntegrationTestRunner() : tests_passed_(0), tests_failed_(0) {}
    
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
        std::cout << std::endl << "=== Integration Test Summary ===" << std::endl;
        std::cout << "Tests passed: " << tests_passed_ << std::endl;
        std::cout << "Tests failed: " << tests_failed_ << std::endl;
        std::cout << "Total tests: " << (tests_passed_ + tests_failed_) << std::endl;
        
        if (tests_failed_ == 0) {
            std::cout << "All integration tests PASSED!" << std::endl;
        }
    }
    
    bool allTestsPassed() const { return tests_failed_ == 0; }
    
private:
    int tests_passed_;
    int tests_failed_;
};

// Performance test for large files
bool testLargeFilePerformance() {
    // Generate a reasonably large test file
    std::ofstream tempFile("large_test.pdb");
    tempFile << "HEADER    LARGE TEST                              01-JAN-23   TEST\n";
    
    for (int i = 1; i <= 5000; ++i) {
        tempFile << "ATOM  " << std::setw(5) << i 
                 << "  CA  ALA A" << std::setw(4) << i 
                 << "      10.000  20.000  30.000  1.00 15.00           C\n";
    }
    tempFile << "END\n";
    tempFile.close();
    
    BioMesh::PDBReader reader;
    
    auto start = std::chrono::high_resolution_clock::now();
    bool result = reader.readFile("large_test.pdb");
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::filesystem::remove("large_test.pdb");
    
    std::cout << "(" << duration.count() << "ms for 5000 atoms) ";
    
    // Should complete within reasonable time (less than 1 second for 5000 atoms)
    return result && reader.getAtomCount() == 5000 && duration.count() < 1000;
}

// Test memory usage with large structures
bool testMemoryHandling() {
    // Create a file with many atoms to test memory handling
    std::ofstream tempFile("memory_test.pdb");
    tempFile << "HEADER    MEMORY TEST                             01-JAN-23   TEST\n";
    
    for (int i = 1; i <= 10000; ++i) {
        tempFile << "ATOM  " << std::setw(5) << i 
                 << "  CA  ALA A" << std::setw(4) << i 
                 << "      " << (i % 100) << ".000  " << (i % 100) << ".000  " << (i % 100) << ".000  1.00 15.00           C\n";
    }
    tempFile << "END\n";
    tempFile.close();
    
    BioMesh::PDBReader reader;
    bool result = reader.readFile("memory_test.pdb");
    
    std::filesystem::remove("memory_test.pdb");
    
    // Test that we can access all atoms
    if (!result || reader.getAtomCount() != 10000) return false;
    
    const auto& atoms = reader.getAtoms();
    bool accessTest = true;
    
    // Sample some atoms to ensure they're properly stored
    for (int i = 0; i < 10000; i += 1000) {
        if (atoms[i].serial != (i + 1)) {
            accessTest = false;
            break;
        }
    }
    
    std::cout << "(10000 atoms stored and accessible) ";
    return accessTest;
}

// Test file format edge cases
bool testFileFormatEdgeCases() {
    bool allPassed = true;
    
    // Test with very long lines
    {
        std::ofstream tempFile("long_line_test.pdb");
        tempFile << "ATOM      1  CA  ALA A   1      10.000  20.000  30.000  1.00 15.00           C  EXTRA_DATA_THAT_SHOULD_BE_IGNORED\n";
        tempFile.close();
        
        BioMesh::PDBReader reader;
        bool result = reader.readFile("long_line_test.pdb");
        std::filesystem::remove("long_line_test.pdb");
        
        if (!result || reader.getAtomCount() != 1) {
            std::cout << "(long line failed) ";
            allPassed = false;
        }
    }
    
    // Test with mixed line endings
    {
        std::ofstream tempFile("mixed_endings_test.pdb", std::ios::binary);
        tempFile << "ATOM      1  CA  ALA A   1      10.000  20.000  30.000  1.00 15.00           C\r\n";
        tempFile << "ATOM      2  CB  ALA A   1      11.000  21.000  31.000  1.00 15.00           C\n";
        tempFile.close();
        
        BioMesh::PDBReader reader;
        bool result = reader.readFile("mixed_endings_test.pdb");
        std::filesystem::remove("mixed_endings_test.pdb");
        
        if (!result || reader.getAtomCount() != 2) {
            std::cout << "(mixed endings failed) ";
            allPassed = false;
        }
    }
    
    // Test with blank lines
    {
        std::ofstream tempFile("blank_lines_test.pdb");
        tempFile << "ATOM      1  CA  ALA A   1      10.000  20.000  30.000  1.00 15.00           C\n";
        tempFile << "\n";
        tempFile << "\n";
        tempFile << "ATOM      2  CB  ALA A   1      11.000  21.000  31.000  1.00 15.00           C\n";
        tempFile.close();
        
        BioMesh::PDBReader reader;
        bool result = reader.readFile("blank_lines_test.pdb");
        std::filesystem::remove("blank_lines_test.pdb");
        
        if (!result || reader.getAtomCount() != 2) {
            std::cout << "(blank lines failed) ";
            allPassed = false;
        }
    }
    
    return allPassed;
}

// Test HETATM limitation documentation
bool testHETATMLimitationDemo() {
    std::ofstream tempFile("hetatm_demo.pdb");
    tempFile << "HEADER    HETATM LIMITATION DEMO                  01-JAN-23   TEST\n";
    tempFile << "ATOM      1  CA  ALA A   1      10.000  20.000  30.000  1.00 15.00           C\n";
    tempFile << "HETATM    2  C1  LIG A 101     15.000  25.000  35.000  1.00 20.00           C\n";
    tempFile << "HETATM    3  C2  LIG A 101     16.000  26.000  36.000  1.00 20.00           C\n";
    tempFile << "ATOM      4  CB  ALA A   1      11.000  21.000  31.000  1.00 15.00           C\n";
    tempFile.close();
    
    BioMesh::PDBReader reader;
    bool result = reader.readFile("hetatm_demo.pdb");
    std::filesystem::remove("hetatm_demo.pdb");
    
    // Should succeed but only parse ATOM records, not HETATM
    bool correctlyLimited = result && 
                           reader.getAtomCount() == 2 &&  // Only 2 ATOM records
                           reader.getWarnings().size() == 2;  // 2 HETATM warnings
    
    if (correctlyLimited) {
        std::cout << "(correctly ignores HETATM records) ";
    } else {
        std::cout << "(HETATM limitation not working properly) ";
    }
    
    return correctlyLimited;
}

// Test coordinate validation and ranges
bool testCoordinateValidation() {
    std::ofstream tempFile("coord_validation_test.pdb");
    tempFile << "HEADER    COORDINATE VALIDATION                   01-JAN-23   TEST\n";
    tempFile << "ATOM      1  CA  ALA A   1    -100.000-200.000 300.000  1.00 15.00           C\n";
    tempFile << "ATOM      2  CB  ALA A   1     100.000 200.000-300.000  1.00 15.00           C\n";
    tempFile.close();
    
    BioMesh::PDBReader reader;
    bool result = reader.readFile("coord_validation_test.pdb");
    std::filesystem::remove("coord_validation_test.pdb");
    
    if (!result || reader.getAtomCount() != 2) return false;
    
    const auto& atoms = reader.getAtoms();
    
    // Check that coordinates are properly parsed including negative values
    bool coordsCorrect = (abs(atoms[0].x - (-100.0)) < 0.001) &&
                        (abs(atoms[0].y - (-200.0)) < 0.001) &&
                        (abs(atoms[0].z - 300.0) < 0.001) &&
                        (abs(atoms[1].x - 100.0) < 0.001) &&
                        (abs(atoms[1].y - 200.0) < 0.001) &&
                        (abs(atoms[1].z - (-300.0)) < 0.001);
    
    // Test coordinate ranges functionality
    std::string ranges = reader.getCoordinateRanges();
    bool rangesCorrect = ranges.find("X: -100.000 to 100.000") != std::string::npos &&
                        ranges.find("Y: -200.000 to 200.000") != std::string::npos &&
                        ranges.find("Z: -300.000 to 300.000") != std::string::npos;
    
    std::cout << "(coordinate parsing and ranges) ";
    return coordsCorrect && rangesCorrect;
}

// Test error accumulation and reporting
bool testErrorReporting() {
    std::ofstream tempFile("error_test.pdb");
    tempFile << "HEADER    ERROR REPORTING TEST                    01-JAN-23   TEST\n";
    tempFile << "ATOM      1  CA  ALA A   1      10.000  20.000  30.000  1.00 15.00           C\n";
    tempFile << "ATOM      2  CB  ALA A\n"; // Too short
    tempFile << "ATOM      3  CC  ALA A   1      invalid coords here  1.00 15.00           C\n";
    tempFile << "ATOM      4  CD  ALA A   1      12.000  22.000  32.000  1.00 15.00           C\n";
    tempFile.close();
    
    BioMesh::PDBReader reader;
    bool result = reader.readFile("error_test.pdb");
    std::filesystem::remove("error_test.pdb");
    
    // Should still succeed overall but accumulate errors
    bool errorHandlingCorrect = result &&
                               reader.getAtomCount() == 2 &&  // Only valid atoms
                               reader.hasErrors() &&
                               reader.getErrors().size() == 2;  // Two error lines
    
    std::cout << "(accumulated " << reader.getErrors().size() << " errors) ";
    return errorHandlingCorrect;
}

int main() {
    std::cout << "=== BioMesh PDBReader Integration Tests ===" << std::endl << std::endl;
    
    IntegrationTestRunner runner;
    
    // Performance tests
    runner.runTest("Large File Performance", testLargeFilePerformance);
    runner.runTest("Memory Handling", testMemoryHandling);
    
    // Edge cases and format handling
    runner.runTest("File Format Edge Cases", testFileFormatEdgeCases);
    runner.runTest("Coordinate Validation", testCoordinateValidation);
    runner.runTest("Error Reporting", testErrorReporting);
    
    // Feature limitation testing
    runner.runTest("HETATM Limitation Demo", testHETATMLimitationDemo);
    
    runner.printSummary();
    
    return runner.allTestsPassed() ? 0 : 1;
}