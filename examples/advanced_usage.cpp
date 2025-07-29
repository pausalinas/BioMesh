#include "PDBReader.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <map>
#include <cmath>
#include <array>

// Function to calculate center of mass
std::array<double, 3> calculateCenterOfMass(const pdb::PDBReader::AtomContainer& atoms) {
    double totalX = 0.0, totalY = 0.0, totalZ = 0.0;
    
    for (const auto& atom : atoms) {
        totalX += atom->getX();
        totalY += atom->getY();
        totalZ += atom->getZ();
    }
    
    double count = static_cast<double>(atoms.size());
    return {totalX / count, totalY / count, totalZ / count};
}

// Function to count atom types
std::map<std::string, int> countAtomTypes(const pdb::PDBReader::AtomContainer& atoms) {
    std::map<std::string, int> counts;
    for (const auto& atom : atoms) {
        counts[atom->getType()]++;
    }
    return counts;
}

// Function to find atoms within a certain distance of a point
std::vector<std::reference_wrapper<const pdb::Atom>> 
findAtomsNear(const pdb::PDBReader::AtomContainer& atoms, 
              double centerX, double centerY, double centerZ, 
              double maxDistance) {
    std::vector<std::reference_wrapper<const pdb::Atom>> nearbyAtoms;
    
    for (const auto& atom : atoms) {
        double dx = atom->getX() - centerX;
        double dy = atom->getY() - centerY;
        double dz = atom->getZ() - centerZ;
        double distance = std::sqrt(dx*dx + dy*dy + dz*dz);
        
        if (distance <= maxDistance) {
            nearbyAtoms.emplace_back(*atom);
        }
    }
    
    return nearbyAtoms;
}

int main() {
    try {
        // Create a PDBReader instance
        pdb::PDBReader reader;
        
        // Parse content directly from string for demonstration
        std::string samplePDB = R"(
HEADER    SAMPLE PROTEIN                           01-JAN-24   SMPL
REMARK   2 RESOLUTION.    1.50 ANGSTROMS.
ATOM      1  N   MET A   1      20.154  16.967  23.416  1.00 11.99           N
ATOM      2  CA  MET A   1      19.030  16.101  23.416  1.00 11.85           C
ATOM      3  C   MET A   1      17.977  16.939  24.133  1.00 11.56           C
ATOM      4  O   MET A   1      17.854  18.147  24.002  1.00 12.03           O
ATOM      5  CB  MET A   1      19.267  14.681  23.964  1.00 12.34           C
ATOM      6  CG  MET A   1      20.626  14.110  23.623  1.00 13.45           C
ATOM      7  SD  MET A   1      20.891  12.552  24.501  1.00 15.67           S
ATOM      8  CE  MET A   1      19.334  11.702  24.456  1.00 14.23           C
ATOM      9  N   ALA A   2      17.154  16.284  24.936  1.00 10.99           N
ATOM     10  CA  ALA A   2      16.022  16.903  25.629  1.00 10.85           C
ATOM     11  C   ALA A   2      14.869  17.139  24.660  1.00 10.56           C
ATOM     12  O   ALA A   2      14.746  16.547  23.598  1.00 11.03           O
ATOM     13  CB  ALA A   2      15.467  16.101  26.764  1.00 11.34           C
END
)";
        
        std::cout << "Parsing sample PDB content...\n";
        auto atoms = reader.parseContent(samplePDB);
        
        std::cout << "Successfully parsed " << reader.getAtomCount() 
                  << " atoms from " << reader.getLinesProcessed() 
                  << " lines.\n\n";
        
        // Atom type analysis
        std::cout << "=== ATOM TYPE ANALYSIS ===\n";
        auto atomCounts = countAtomTypes(atoms);
        for (const auto& [type, count] : atomCounts) {
            std::cout << std::setw(4) << type << ": " << count << " atoms\n";
        }
        std::cout << "\n";
        
        // Center of mass calculation
        std::cout << "=== CENTER OF MASS ===\n";
        auto com = calculateCenterOfMass(atoms);
        std::cout << std::fixed << std::setprecision(3);
        std::cout << "Center of mass: (" << com[0] << ", " << com[1] << ", " << com[2] << ")\n\n";
        
        // Distance analysis from center of mass
        std::cout << "=== DISTANCE FROM CENTER OF MASS ===\n";
        std::vector<std::pair<double, const pdb::Atom*>> distances;
        for (const auto& atom : atoms) {
            double dx = atom->getX() - com[0];
            double dy = atom->getY() - com[1];
            double dz = atom->getZ() - com[2];
            double distance = std::sqrt(dx*dx + dy*dy + dz*dz);
            distances.emplace_back(distance, atom.get());
        }
        
        // Sort by distance
        std::sort(distances.begin(), distances.end());
        
        std::cout << "Closest 5 atoms to center of mass:\n";
        std::cout << std::setw(5) << "Rank" << std::setw(8) << "Type" 
                  << std::setw(12) << "Distance" << "\n";
        std::cout << std::string(25, '-') << "\n";
        
        for (size_t i = 0; i < std::min(distances.size(), size_t(5)); ++i) {
            std::cout << std::setw(5) << (i+1) 
                      << std::setw(8) << distances[i].second->getType()
                      << std::setw(12) << distances[i].first << "\n";
        }
        std::cout << "\n";
        
        // Find atoms within a certain radius
        std::cout << "=== SPATIAL SEARCH ===\n";
        double searchRadius = 5.0;
        auto nearbyAtoms = findAtomsNear(atoms, com[0], com[1], com[2], searchRadius);
        
        std::cout << "Atoms within " << searchRadius << " Angstroms of center of mass: " 
                  << nearbyAtoms.size() << "\n";
        
        for (const auto& atomRef : nearbyAtoms) {
            const auto& atom = atomRef.get();
            std::cout << "  " << atom.getType() << " (ID: " << atom.getId() << ")\n";
        }
        std::cout << "\n";
        
        // Pairwise distance analysis
        std::cout << "=== PAIRWISE DISTANCES ===\n";
        std::cout << "Distance matrix (first 4 atoms):\n";
        std::cout << std::setw(8) << "";
        for (size_t i = 0; i < std::min(atoms.size(), size_t(4)); ++i) {
            std::cout << std::setw(8) << atoms[i]->getType();
        }
        std::cout << "\n";
        
        for (size_t i = 0; i < std::min(atoms.size(), size_t(4)); ++i) {
            std::cout << std::setw(8) << atoms[i]->getType();
            for (size_t j = 0; j < std::min(atoms.size(), size_t(4)); ++j) {
                if (i == j) {
                    std::cout << std::setw(8) << "0.000";
                } else {
                    double dist = atoms[i]->distanceTo(*atoms[j]);
                    std::cout << std::setw(8) << std::setprecision(3) << dist;
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        
        // Statistical summary
        std::cout << "=== STATISTICAL SUMMARY ===\n";
        
        // Find min/max distances from origin
        auto minMaxDist = std::minmax_element(atoms.begin(), atoms.end(),
            [](const auto& a, const auto& b) {
                return a->distanceFromOrigin() < b->distanceFromOrigin();
            });
        
        std::cout << "Distance from origin:\n";
        std::cout << "  Min: " << (*minMaxDist.first)->distanceFromOrigin() 
                  << " (atom " << (*minMaxDist.first)->getType() << ")\n";
        std::cout << "  Max: " << (*minMaxDist.second)->distanceFromOrigin() 
                  << " (atom " << (*minMaxDist.second)->getType() << ")\n";
        
        // Calculate average distance from origin
        double totalDist = 0.0;
        for (const auto& atom : atoms) {
            totalDist += atom->distanceFromOrigin();
        }
        std::cout << "  Avg: " << totalDist / atoms.size() << "\n\n";
        
        std::cout << "Analysis complete!\n";
        
    } catch (const pdb::PDBFileException& e) {
        std::cerr << "File error: " << e.what() << "\n";
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