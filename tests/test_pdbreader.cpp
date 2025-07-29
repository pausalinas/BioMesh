#include <gtest/gtest.h>
#include "PDBReader.h"
#include <fstream>
#include <filesystem>

using namespace pdb;

class PDBReaderTest : public ::testing::Test {
protected:
    void SetUp() override {
        reader_ = std::make_unique<PDBReader>();
        
        // Create temporary directory for test files
        test_dir_ = std::filesystem::temp_directory_path() / "pdb_test";
        std::filesystem::create_directories(test_dir_);
    }

    void TearDown() override {
        // Clean up test files
        std::error_code ec;
        std::filesystem::remove_all(test_dir_, ec);
    }

    void createTestFile(const std::string& filename, const std::string& content) {
        std::filesystem::path filepath = test_dir_ / filename;
        std::ofstream file(filepath);
        file << content;
        file.close();
    }

    std::string getTestFilePath(const std::string& filename) {
        return (test_dir_ / filename).string();
    }

    std::unique_ptr<PDBReader> reader_;
    std::filesystem::path test_dir_;
};

TEST_F(PDBReaderTest, ValidAtomLine) {
    EXPECT_TRUE(PDBReader::isValidAtomLine(
        "ATOM      1  N   ALA A   1      20.154  16.967  23.416  1.00 11.99           N  "));
    EXPECT_TRUE(PDBReader::isValidAtomLine(
        "ATOM      2  CA  ALA A   1      19.030  16.101  23.416  1.00 11.85           C  "));
}

TEST_F(PDBReaderTest, InvalidAtomLine) {
    EXPECT_FALSE(PDBReader::isValidAtomLine("HEADER    LYSOZYME                               25-AUG-77   2LYZ"));
    EXPECT_FALSE(PDBReader::isValidAtomLine("REMARK   2 RESOLUTION.    2.00 ANGSTROMS."));
    EXPECT_FALSE(PDBReader::isValidAtomLine("HETATM   46  O   HOH A   7      -6.471   5.227   7.124  1.00 10.80           O  "));
    EXPECT_FALSE(PDBReader::isValidAtomLine("ATOM      1  N   ALA")); // Too short
    EXPECT_FALSE(PDBReader::isValidAtomLine("")); // Empty line
}

TEST_F(PDBReaderTest, ParseValidContent) {
    std::string content = 
        "HEADER    TEST PROTEIN                           01-JAN-24   TEST\n"
        "ATOM      1  N   ALA A   1      20.154  16.967  23.416  1.00 11.99           N\n"
        "ATOM      2  CA  ALA A   1      19.030  16.101  23.416  1.00 11.85           C\n"
        "ATOM      3  C   ALA A   1      17.977  16.939  24.133  1.00 11.56           C\n"
        "END\n";

    auto atoms = reader_->parseContent(content);

    EXPECT_EQ(atoms.size(), 3);
    EXPECT_EQ(reader_->getAtomCount(), 3);
    EXPECT_EQ(reader_->getLinesProcessed(), 5);

    // Check first atom
    EXPECT_EQ(atoms[0]->getId(), 1);
    EXPECT_EQ(atoms[0]->getType(), "N");
    EXPECT_DOUBLE_EQ(atoms[0]->getX(), 20.154);
    EXPECT_DOUBLE_EQ(atoms[0]->getY(), 16.967);
    EXPECT_DOUBLE_EQ(atoms[0]->getZ(), 23.416);

    // Check second atom
    EXPECT_EQ(atoms[1]->getId(), 2);
    EXPECT_EQ(atoms[1]->getType(), "CA");
    EXPECT_DOUBLE_EQ(atoms[1]->getX(), 19.030);
    EXPECT_DOUBLE_EQ(atoms[1]->getY(), 16.101);
    EXPECT_DOUBLE_EQ(atoms[1]->getZ(), 23.416);
}

TEST_F(PDBReaderTest, ParseEmptyContent) {
    auto atoms = reader_->parseContent("");
    EXPECT_EQ(atoms.size(), 0);
    EXPECT_EQ(reader_->getAtomCount(), 0);
    EXPECT_EQ(reader_->getLinesProcessed(), 0);
}

TEST_F(PDBReaderTest, ParseContentWithoutAtoms) {
    std::string content = 
        "HEADER    TEST PROTEIN                           01-JAN-24   TEST\n"
        "REMARK   2 RESOLUTION.    2.00 ANGSTROMS.\n"
        "END\n";

    auto atoms = reader_->parseContent(content);
    EXPECT_EQ(atoms.size(), 0);
    EXPECT_EQ(reader_->getAtomCount(), 0);
    EXPECT_EQ(reader_->getLinesProcessed(), 3);
}

TEST_F(PDBReaderTest, ParseContentWithEmptyLines) {
    std::string content = 
        "HEADER    TEST PROTEIN\n"
        "\n"
        "ATOM      1  N   ALA A   1      20.154  16.967  23.416  1.00 11.99           N\n"
        "\n"
        "ATOM      2  CA  ALA A   1      19.030  16.101  23.416  1.00 11.85           C\n"
        "\n";

    auto atoms = reader_->parseContent(content);
    EXPECT_EQ(atoms.size(), 2);
    EXPECT_EQ(reader_->getAtomCount(), 2);
}

TEST_F(PDBReaderTest, ParseInvalidAtomLine) {
    std::string content = 
        "ATOM      1  N   ALA A   1      invalid  16.967  23.416  1.00 11.99           N\n";

    EXPECT_THROW(reader_->parseContent(content), PDBParsingException);
}

TEST_F(PDBReaderTest, ParseShortAtomLine) {
    std::string content = "ATOM      1  N   ALA A   1      20.154\n";

    // Short lines are silently skipped, so no atoms should be parsed
    auto atoms = reader_->parseContent(content);
    EXPECT_EQ(atoms.size(), 0);
    EXPECT_EQ(reader_->getAtomCount(), 0);
}

TEST_F(PDBReaderTest, ParseAtomLineWithEmptyCoordinates) {
    std::string content = 
        "ATOM      1  N   ALA A   1              16.967  23.416  1.00 11.99           N\n";

    EXPECT_THROW(reader_->parseContent(content), PDBParsingException);
}

TEST_F(PDBReaderTest, ParseAtomLineWithEmptyAtomType) {
    std::string content = 
        "ATOM      1      ALA A   1      20.154  16.967  23.416  1.00 11.99           N\n";

    EXPECT_THROW(reader_->parseContent(content), PDBParsingException);
}

TEST_F(PDBReaderTest, ReadValidFile) {
    std::string content = 
        "HEADER    TEST PROTEIN\n"
        "ATOM      1  N   ALA A   1      20.154  16.967  23.416  1.00 11.99           N\n"
        "ATOM      2  CA  ALA A   1      19.030  16.101  23.416  1.00 11.85           C\n"
        "END\n";

    createTestFile("valid.pdb", content);
    
    auto atoms = reader_->readFile(getTestFilePath("valid.pdb"));
    EXPECT_EQ(atoms.size(), 2);
    EXPECT_EQ(reader_->getAtomCount(), 2);
}

TEST_F(PDBReaderTest, ReadNonExistentFile) {
    EXPECT_THROW(reader_->readFile(getTestFilePath("nonexistent.pdb")), PDBFileException);
}

TEST_F(PDBReaderTest, ReadEmptyFile) {
    createTestFile("empty.pdb", "");
    
    auto atoms = reader_->readFile(getTestFilePath("empty.pdb"));
    EXPECT_EQ(atoms.size(), 0);
    EXPECT_EQ(reader_->getAtomCount(), 0);
}

TEST_F(PDBReaderTest, NegativeCoordinates) {
    std::string content = 
        "ATOM      1  N   ALA A   1     -20.154 -16.967 -23.416  1.00 11.99           N\n";

    auto atoms = reader_->parseContent(content);
    EXPECT_EQ(atoms.size(), 1);
    EXPECT_DOUBLE_EQ(atoms[0]->getX(), -20.154);
    EXPECT_DOUBLE_EQ(atoms[0]->getY(), -16.967);
    EXPECT_DOUBLE_EQ(atoms[0]->getZ(), -23.416);
}

TEST_F(PDBReaderTest, LargeCoordinates) {
    std::string content = 
        "ATOM      1  N   ALA A   1    1234.5672345.6783456.789  1.00 11.99           N\n";

    auto atoms = reader_->parseContent(content);
    EXPECT_EQ(atoms.size(), 1);
    EXPECT_DOUBLE_EQ(atoms[0]->getX(), 1234.567);
    EXPECT_DOUBLE_EQ(atoms[0]->getY(), 2345.678);
    EXPECT_DOUBLE_EQ(atoms[0]->getZ(), 3456.789);
}

TEST_F(PDBReaderTest, SmallCoordinates) {
    std::string content = 
        "ATOM      1  N   ALA A   1       0.001   0.002   0.003  1.00 11.99           N\n";

    auto atoms = reader_->parseContent(content);
    EXPECT_EQ(atoms.size(), 1);
    EXPECT_DOUBLE_EQ(atoms[0]->getX(), 0.001);
    EXPECT_DOUBLE_EQ(atoms[0]->getY(), 0.002);
    EXPECT_DOUBLE_EQ(atoms[0]->getZ(), 0.003);
}

TEST_F(PDBReaderTest, SequentialAtomIds) {
    std::string content = 
        "ATOM     99  N   ALA A   1      20.154  16.967  23.416  1.00 11.99           N\n"
        "ATOM    100  CA  ALA A   1      19.030  16.101  23.416  1.00 11.85           C\n"
        "ATOM    101  C   ALA A   1      17.977  16.939  24.133  1.00 11.56           C\n";

    auto atoms = reader_->parseContent(content);
    EXPECT_EQ(atoms.size(), 3);
    
    // Sequential IDs should be assigned regardless of PDB atom numbers
    EXPECT_EQ(atoms[0]->getId(), 1);
    EXPECT_EQ(atoms[1]->getId(), 2);
    EXPECT_EQ(atoms[2]->getId(), 3);
}

TEST_F(PDBReaderTest, DifferentAtomTypes) {
    std::string content = 
        "ATOM      1  N   ALA A   1      20.154  16.967  23.416  1.00 11.99           N\n"
        "ATOM      2  CA  ALA A   1      19.030  16.101  23.416  1.00 11.85           C\n"
        "ATOM      3  CB  ALA A   1      17.977  16.939  24.133  1.00 11.56           C\n"
        "ATOM      4  O   ALA A   1      16.977  17.939  25.133  1.00 10.56           O\n";

    auto atoms = reader_->parseContent(content);
    EXPECT_EQ(atoms.size(), 4);
    EXPECT_EQ(atoms[0]->getType(), "N");
    EXPECT_EQ(atoms[1]->getType(), "CA");
    EXPECT_EQ(atoms[2]->getType(), "CB");
    EXPECT_EQ(atoms[3]->getType(), "O");
}

TEST_F(PDBReaderTest, MoveSemantics) {
    std::string content = 
        "ATOM      1  N   ALA A   1      20.154  16.967  23.416  1.00 11.99           N\n";

    auto reader1 = std::make_unique<PDBReader>();
    auto atoms1 = reader1->parseContent(content);
    
    // Move reader
    auto reader2 = std::move(reader1);
    EXPECT_EQ(reader2->getAtomCount(), 1);
    
    // Move atoms container
    auto atoms2 = std::move(atoms1);
    EXPECT_EQ(atoms2.size(), 1);
    EXPECT_EQ(atoms2[0]->getType(), "N");
}

TEST_F(PDBReaderTest, MultipleParseOperations) {
    std::string content1 = 
        "ATOM      1  N   ALA A   1      20.154  16.967  23.416  1.00 11.99           N\n";
    std::string content2 = 
        "ATOM      1  CA  ALA A   1      19.030  16.101  23.416  1.00 11.85           C\n"
        "ATOM      2  C   ALA A   1      17.977  16.939  24.133  1.00 11.56           C\n";

    auto atoms1 = reader_->parseContent(content1);
    EXPECT_EQ(atoms1.size(), 1);
    EXPECT_EQ(reader_->getAtomCount(), 1);

    auto atoms2 = reader_->parseContent(content2);
    EXPECT_EQ(atoms2.size(), 2);
    EXPECT_EQ(reader_->getAtomCount(), 2); // Should be updated
}

TEST_F(PDBReaderTest, CoordinateOutOfRange) {
    std::string content = 
        "ATOM      1  N   ALA A   1  1.0e308  16.967  23.416  1.00 11.99           N\n";

    EXPECT_THROW(reader_->parseContent(content), PDBParsingException);
}

TEST_F(PDBReaderTest, WhitespaceHandling) {
    std::string content = 
        "ATOM      1   N  ALA A   1      20.154  16.967  23.416  1.00 11.99           N\n";

    auto atoms = reader_->parseContent(content);
    EXPECT_EQ(atoms.size(), 1);
    EXPECT_EQ(atoms[0]->getType(), "N"); // Should be trimmed
}