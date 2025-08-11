#include "biomesh/biomesh.h"
#include <iostream>
#include <iomanip>

using namespace BioMesh;

void printBox(const BoundingBox& box, const std::string& name) {
    std::cout << name << ": ";
    if (box.isEmpty()) {
        std::cout << "EMPTY" << std::endl;
    } else {
        std::cout << "["
                  << std::fixed << std::setprecision(1)
                  << box.getMinX() << "," << box.getMinY() << "," << box.getMinZ()
                  << "] to ["
                  << box.getMaxX() << "," << box.getMaxY() << "," << box.getMaxZ()
                  << "] volume=" << box.getVolume() << std::endl;
    }
}

int main() {
    std::cout << "=== BioMesh Enhanced BoundingBox Demo ===" << std::endl;
    
    // Create some atoms for a molecular system
    std::vector<Atom> atoms;
    atoms.emplace_back(-2.0, -1.5, -1.0, "C");
    atoms.emplace_back(1.5, 2.0, 0.5, "N");
    atoms.emplace_back(0.0, 0.0, 2.0, "O");
    atoms.emplace_back(-1.0, 1.0, -0.5, "H");
    
    // Calculate bounding box for the molecular system
    BoundingBox molecularBox;
    molecularBox.calculateFromAtoms(atoms);
    printBox(molecularBox, "Molecular System");
    
    // Demonstrate subdivision for octree-based mesh generation
    std::cout << "\n=== Octree Subdivision ===" << std::endl;
    auto octants = molecularBox.subdivide();
    
    for (size_t i = 0; i < octants.size(); ++i) {
        printBox(octants[i], "Octant " + std::to_string(i));
    }
    
    // Demonstrate containment and intersection testing
    std::cout << "\n=== Spatial Queries ===" << std::endl;
    
    // Create a query region
    BoundingBox queryBox(-1.0, -1.0, -1.0, 1.0, 1.0, 1.0);
    printBox(queryBox, "Query Region");
    
    // Test containment
    std::cout << "\nContainment tests:" << std::endl;
    std::cout << "Molecular box contains query box: " 
              << (molecularBox.contains(queryBox) ? "YES" : "NO") << std::endl;
    std::cout << "Query box contains molecular box: " 
              << (queryBox.contains(molecularBox) ? "YES" : "NO") << std::endl;
    
    // Test intersection
    std::cout << "\nIntersection tests:" << std::endl;
    std::cout << "Molecular box intersects query box: " 
              << (molecularBox.intersects(queryBox) ? "YES" : "NO") << std::endl;
    
    // Count intersecting octants
    int intersectingOctants = 0;
    for (size_t i = 0; i < octants.size(); ++i) {
        if (octants[i].intersects(queryBox)) {
            std::cout << "Octant " << i << " intersects query box" << std::endl;
            intersectingOctants++;
        }
    }
    std::cout << "Total intersecting octants: " << intersectingOctants << std::endl;
    
    // Demonstrate atom containment within octants
    std::cout << "\n=== Atom Distribution in Octants ===" << std::endl;
    for (size_t i = 0; i < octants.size(); ++i) {
        int atomCount = 0;
        for (const auto& atom : atoms) {
            if (octants[i].contains(atom)) {
                atomCount++;
            }
        }
        if (atomCount > 0) {
            std::cout << "Octant " << i << " contains " << atomCount << " atoms" << std::endl;
        }
    }
    
    return 0;
}