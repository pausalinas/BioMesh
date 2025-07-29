#include "PDBReader.h"
#include <iostream>
#include <iomanip>

int main() {
    try {
        // Create a PDBReader instance
        pdb::PDBReader reader;
        
        // Read a PDB file
        std::cout << "Reading sample.pdb...\n";
        auto atoms = reader.readFile("sample.pdb");
        
        // Display statistics
        std::cout << "Successfully parsed " << reader.getAtomCount() 
                  << " atoms from " << reader.getLinesProcessed() 
                  << " lines.\n\n";
        
        // Display first few atoms
        std::cout << "First 5 atoms:\n";
        std::cout << std::string(70, '-') << "\n";
        std::cout << std::left << std::setw(5) << "ID" 
                  << std::setw(8) << "Type" 
                  << std::setw(12) << "X" 
                  << std::setw(12) << "Y" 
                  << std::setw(12) << "Z" 
                  << std::setw(15) << "Dist from Origin" << "\n";
        std::cout << std::string(70, '-') << "\n";
        
        for (size_t i = 0; i < std::min(atoms.size(), size_t(5)); ++i) {
            const auto& atom = atoms[i];
            std::cout << std::left << std::setw(5) << atom->getId()
                      << std::setw(8) << atom->getType()
                      << std::right << std::fixed << std::setprecision(3)
                      << std::setw(12) << atom->getX()
                      << std::setw(12) << atom->getY()
                      << std::setw(12) << atom->getZ()
                      << std::setw(15) << atom->distanceFromOrigin() << "\n";
        }
        
        if (atoms.size() > 5) {
            std::cout << "... and " << (atoms.size() - 5) << " more atoms\n";
        }
        
        // Calculate some basic statistics
        if (!atoms.empty()) {
            double minX = atoms[0]->getX(), maxX = atoms[0]->getX();
            double minY = atoms[0]->getY(), maxY = atoms[0]->getY();
            double minZ = atoms[0]->getZ(), maxZ = atoms[0]->getZ();
            
            for (const auto& atom : atoms) {
                minX = std::min(minX, atom->getX());
                maxX = std::max(maxX, atom->getX());
                minY = std::min(minY, atom->getY());
                maxY = std::max(maxY, atom->getY());
                minZ = std::min(minZ, atom->getZ());
                maxZ = std::max(maxZ, atom->getZ());
            }
            
            std::cout << "\nBounding box:\n";
            std::cout << "X: [" << std::fixed << std::setprecision(3) 
                      << minX << ", " << maxX << "]\n";
            std::cout << "Y: [" << minY << ", " << maxY << "]\n";
            std::cout << "Z: [" << minZ << ", " << maxZ << "]\n";
        }
        
    } catch (const pdb::PDBFileException& e) {
        std::cerr << "File error: " << e.what() << "\n";
        std::cerr << "Make sure 'sample.pdb' exists in the current directory.\n";
        return 1;
    } catch (const pdb::PDBParsingException& e) {
        std::cerr << "Parsing error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected error: " << e.what() << "\n";
        return 1;
    }
    
    return 0;
}