#include "pdb_reader.h"

int main(int argc, char* argv[]) {
    // Check command line arguments
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }
    
    std::string filename = argv[1];
    PDBData pdbData;
    
    // Read the PDB file
    if (!readPDBFile(filename, pdbData)) {
        std::cerr << "Failed to read PDB file: " << filename << std::endl;
        return 1;
    }
    
    // Display summary information
    pdbData.printSummary();
    
    // Ask user if they want to see detailed atom information
    std::cout << "\nWould you like to see detailed atom information? (y/n): ";
    char choice;
    std::cin >> choice;
    
    if (choice == 'y' || choice == 'Y') {
        pdbData.printAtoms();
    }
    
    return 0;
}