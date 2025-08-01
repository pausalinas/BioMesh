#include "PDBReader.h"
#include <iostream>
#include <string>
#include <iomanip>

void printUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [options] <pdb_file>" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -v, --verbose    Enable verbose output" << std::endl;
    std::cout << "  -s, --stats      Show detailed statistics" << std::endl;
    std::cout << "  -h, --help       Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << programName << " protein.pdb" << std::endl;
    std::cout << "  " << programName << " --verbose --stats protein.pdb" << std::endl;
}

int main(int argc, char* argv[]) {
    bool verbose = false;
    bool showStats = false;
    std::string filename;
    
    // Parse command line arguments
    for (int i = 1; i < argc; ++i) {
        std::string arg(argv[i]);
        
        if (arg == "-h" || arg == "--help") {
            printUsage(argv[0]);
            return 0;
        } else if (arg == "-v" || arg == "--verbose") {
            verbose = true;
        } else if (arg == "-s" || arg == "--stats") {
            showStats = true;
        } else if (arg[0] != '-') {
            if (filename.empty()) {
                filename = arg;
            } else {
                std::cerr << "Error: Multiple filenames provided" << std::endl;
                printUsage(argv[0]);
                return 1;
            }
        } else {
            std::cerr << "Error: Unknown option " << arg << std::endl;
            printUsage(argv[0]);
            return 1;
        }
    }
    
    if (filename.empty()) {
        std::cerr << "Error: No PDB file specified" << std::endl;
        printUsage(argv[0]);
        return 1;
    }
    
    if (verbose) {
        std::cout << "Processing PDB file: " << filename << std::endl;
    }
    
    BioMesh::PDBReader reader;
    
    if (!reader.readFile(filename)) {
        std::cerr << "Error: Failed to read PDB file: " << filename << std::endl;
        
        const auto& errors = reader.getErrors();
        for (const auto& error : errors) {
            std::cerr << "  " << error << std::endl;
        }
        return 1;
    }
    
    // Basic output
    std::cout << "Successfully parsed PDB file: " << filename << std::endl;
    std::cout << "Found " << reader.getAtomCount() << " ATOM records" << std::endl;
    
    // Show warnings if any
    const auto& warnings = reader.getWarnings();
    if (!warnings.empty()) {
        std::cout << "Warnings (" << warnings.size() << "):" << std::endl;
        for (const auto& warning : warnings) {
            std::cout << "  " << warning << std::endl;
        }
    }
    
    // Verbose output
    if (verbose) {
        std::cout << std::endl << "=== Verbose Output ===" << std::endl;
        const auto& atoms = reader.getAtoms();
        
        if (!atoms.empty()) {
            std::cout << "First 10 atoms:" << std::endl;
            std::cout << std::setw(6) << "Serial" 
                      << std::setw(5) << "Name"
                      << std::setw(4) << "Res"
                      << std::setw(2) << "Ch"
                      << std::setw(5) << "ResN"
                      << std::setw(9) << "X"
                      << std::setw(9) << "Y"
                      << std::setw(9) << "Z" << std::endl;
            
            for (size_t i = 0; i < std::min(atoms.size(), size_t(10)); ++i) {
                const auto& atom = atoms[i];
                std::cout << std::setw(6) << atom.serial
                          << std::setw(5) << atom.name
                          << std::setw(4) << atom.resName
                          << std::setw(2) << atom.chainID
                          << std::setw(5) << atom.resSeq
                          << std::setw(9) << std::fixed << std::setprecision(3) << atom.x
                          << std::setw(9) << std::fixed << std::setprecision(3) << atom.y
                          << std::setw(9) << std::fixed << std::setprecision(3) << atom.z
                          << std::endl;
            }
            
            if (atoms.size() > 10) {
                std::cout << "... and " << (atoms.size() - 10) << " more atoms" << std::endl;
            }
        }
    }
    
    // Show detailed statistics
    if (showStats) {
        std::cout << std::endl;
        reader.printStatistics();
    }
    
    return 0;
}