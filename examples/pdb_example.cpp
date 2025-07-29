#include "PDBReader.h"
#include <iostream>
#include <iomanip>

/**
 * @brief Example program demonstrating PDBReader usage
 */
int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <pdb_file>" << std::endl;
        return 1;
    }

    const std::string filename(argv[1]);
    
    try {
        // Create PDBReader instance using RAII
        BioMesh::PDBReader reader(filename);
        
        std::cout << "Parsing PDB file: " << filename << std::endl;
        
        // Parse the file
        reader.parse();
        
        std::cout << "Successfully parsed " << reader.getAtomCount() << " atoms." << std::endl;
        std::cout << std::endl;

        // Display parsed atoms
        const auto& atoms = reader.getAtoms();
        
        std::cout << std::setw(6) << "Serial" 
                  << std::setw(6) << "Atom" 
                  << std::setw(6) << "Res" 
                  << std::setw(3) << "Ch" 
                  << std::setw(6) << "ResNum"
                  << std::setw(10) << "X" 
                  << std::setw(10) << "Y" 
                  << std::setw(10) << "Z" << std::endl;
        
        std::cout << std::string(60, '-') << std::endl;

        // Display first 10 atoms as example
        size_t display_count = std::min(size_t(10), atoms.size());
        
        for (size_t i = 0; i < display_count; ++i) {
            const auto& atom = atoms[i];
            std::cout << std::setw(6) << atom->serial_number
                      << std::setw(6) << atom->atom_name
                      << std::setw(6) << atom->residue_name
                      << std::setw(3) << atom->chain_id
                      << std::setw(6) << atom->residue_number
                      << std::setw(10) << std::fixed << std::setprecision(3) << atom->x
                      << std::setw(10) << std::fixed << std::setprecision(3) << atom->y
                      << std::setw(10) << std::fixed << std::setprecision(3) << atom->z
                      << std::endl;
        }
        
        if (atoms.size() > 10) {
            std::cout << "... and " << (atoms.size() - 10) << " more atoms." << std::endl;
        }

    } catch (const BioMesh::PDBFileException& e) {
        std::cerr << "File error: " << e.what() << std::endl;
        return 1;
    } catch (const BioMesh::PDBParseException& e) {
        std::cerr << "Parse error: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}