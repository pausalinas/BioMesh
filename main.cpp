#include "pdb_reader.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "=== BioMesh PDB File Reader ===" << std::endl;
    std::cout << "A tool to read and parse PDB (Protein Data Bank) files" << std::endl;
    std::cout << std::endl;
    
    // Check command line arguments
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <pdb_file>" << std::endl;
        std::cout << "Example: " << argv[0] << " sample.pdb" << std::endl;
        return 1;
    }
    
    std::string filename = argv[1];
    
    // Create PDB reader instance
    PDBReader reader;
    
    // Try to open and parse the file
    std::cout << "Attempting to open file: " << filename << std::endl;
    
    if (!reader.openFile(filename)) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return 1;
    }
    
    if (!reader.parseFile()) {
        std::cerr << "Failed to parse PDB file: " << filename << std::endl;
        return 1;
    }
    
    // Display summary
    reader.printSummary();
    
    // Ask user if they want to see detailed atom information
    char choice;
    std::cout << "\nWould you like to see detailed atom information? (y/n): ";
    std::cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        reader.printAtoms();
    }
    
    std::cout << "\nPDB file processing completed successfully!" << std::endl;
    
    return 0;
}