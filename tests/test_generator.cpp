#include "PDBReader.h"
#include <iostream>
#include <fstream>
#include <random>
#include <iomanip>
#include <string>

class PDBTestDataGenerator {
public:
    PDBTestDataGenerator() : rng_(std::random_device{}()) {}
    
    void generateLargeProtein(const std::string& filename, int numResidues = 1000) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error: Cannot create file " << filename << std::endl;
            return;
        }
        
        file << "HEADER    LARGE PROTEIN                           01-JAN-23   TEST" << std::endl;
        file << "TITLE     LARGE PROTEIN STRUCTURE FOR PERFORMANCE TESTING" << std::endl;
        
        std::vector<std::string> aminoAcids = {
            "ALA", "ARG", "ASN", "ASP", "CYS", "GLN", "GLU", "GLY",
            "HIS", "ILE", "LEU", "LYS", "MET", "PHE", "PRO", "SER",
            "THR", "TRP", "TYR", "VAL"
        };
        
        std::vector<std::string> backboneAtoms = {"N", "CA", "C", "O"};
        std::vector<char> chains = {'A', 'B', 'C', 'D'};
        
        int atomSerial = 1;
        std::uniform_real_distribution<double> coordDist(-50.0, 50.0);
        std::uniform_real_distribution<double> bFactorDist(10.0, 30.0);
        std::uniform_int_distribution<int> residueDist(0, aminoAcids.size() - 1);
        std::uniform_int_distribution<int> chainDist(0, chains.size() - 1);
        
        for (int res = 1; res <= numResidues; ++res) {
            std::string resName = aminoAcids[residueDist(rng_)];
            char chain = chains[chainDist(rng_)];
            
            // Add backbone atoms
            for (const auto& atomName : backboneAtoms) {
                file << "ATOM  " 
                     << std::setw(5) << atomSerial
                     << "  " << std::setw(4) << std::left << atomName
                     << resName << " " << chain 
                     << std::setw(4) << res << "    "
                     << std::setw(8) << std::fixed << std::setprecision(3) << coordDist(rng_)
                     << std::setw(8) << std::fixed << std::setprecision(3) << coordDist(rng_)
                     << std::setw(8) << std::fixed << std::setprecision(3) << coordDist(rng_)
                     << "  1.00" << std::setw(6) << std::fixed << std::setprecision(2) << bFactorDist(rng_)
                     << "           " << atomName.substr(0, 1) << std::endl;
                atomSerial++;
            }
            
            // Add side chain atoms for some residues
            if (resName != "GLY" && res % 3 == 0) { // Add CB for every 3rd non-glycine
                file << "ATOM  " 
                     << std::setw(5) << atomSerial
                     << "  " << std::setw(4) << std::left << "CB"
                     << resName << " " << chain 
                     << std::setw(4) << res << "    "
                     << std::setw(8) << std::fixed << std::setprecision(3) << coordDist(rng_)
                     << std::setw(8) << std::fixed << std::setprecision(3) << coordDist(rng_)
                     << std::setw(8) << std::fixed << std::setprecision(3) << coordDist(rng_)
                     << "  1.00" << std::setw(6) << std::fixed << std::setprecision(2) << bFactorDist(rng_)
                     << "           C" << std::endl;
                atomSerial++;
            }
        }
        
        file << "END" << std::endl;
        file.close();
        
        std::cout << "Generated large protein with " << numResidues 
                  << " residues (" << (atomSerial - 1) << " atoms) in " << filename << std::endl;
    }
    
    void generateEdgeCases(const std::string& baseDir) {
        // Empty file
        {
            std::ofstream file(baseDir + "/empty.pdb");
            file.close();
        }
        
        // File with only headers
        {
            std::ofstream file(baseDir + "/headers_only.pdb");
            file << "HEADER    HEADERS ONLY                            01-JAN-23   TEST" << std::endl;
            file << "TITLE     FILE WITH NO ATOM RECORDS" << std::endl;
            file << "REMARK   This file has no ATOM or HETATM records" << std::endl;
            file << "END" << std::endl;
            file.close();
        }
        
        // File with extreme coordinates
        {
            std::ofstream file(baseDir + "/extreme_coords.pdb");
            file << "HEADER    EXTREME COORDINATES                     01-JAN-23   TEST" << std::endl;
            file << "ATOM      1  CA  ALA A   1    -999.999-999.999-999.999  1.00 99.99           C" << std::endl;
            file << "ATOM      2  CA  ALA A   2     999.999 999.999 999.999  1.00 99.99           C" << std::endl;
            file << "END" << std::endl;
            file.close();
        }
        
        // File with missing coordinates
        {
            std::ofstream file(baseDir + "/missing_coords.pdb");
            file << "HEADER    MISSING COORDINATES                     01-JAN-23   TEST" << std::endl;
            file << "ATOM      1  CA  ALA A   1                              1.00 10.00           C" << std::endl;
            file << "ATOM      2  CA  ALA A   2       0.000   0.000          1.00 10.00           C" << std::endl;
            file << "END" << std::endl;
            file.close();
        }
        
        std::cout << "Generated edge case test files in " << baseDir << std::endl;
    }
    
private:
    std::mt19937 rng_;
};

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  --large <filename> <size>  Generate large protein (default size: 1000)" << std::endl;
    std::cout << "  --edge-cases <directory>   Generate edge case test files" << std::endl;
    std::cout << "  --all <directory>          Generate all test files" << std::endl;
    std::cout << "  -h, --help                 Show this help message" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    PDBTestDataGenerator generator;
    
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "--large") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --large requires filename" << std::endl;
                return 1;
            }
            std::string filename = argv[++i];
            int size = 1000;
            if (i + 1 < argc && argv[i + 1][0] != '-') {
                size = std::stoi(argv[++i]);
            }
            generator.generateLargeProtein(filename, size);
        } else if (arg == "--edge-cases") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --edge-cases requires directory" << std::endl;
                return 1;
            }
            std::string directory = argv[++i];
            generator.generateEdgeCases(directory);
        } else if (arg == "--all") {
            if (i + 1 >= argc) {
                std::cerr << "Error: --all requires directory" << std::endl;
                return 1;
            }
            std::string directory = argv[++i];
            generator.generateLargeProtein(directory + "/large_protein.pdb", 1000);
            generator.generateEdgeCases(directory);
        }
    }
    
    return 0;
}