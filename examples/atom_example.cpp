#include <iostream>
#include <vector>
#include <iomanip>
#include "Atom.h"
#include "AtomBuilder.h"
#include "BoundingBox.h"

using namespace BioMesh;

void printAtom(const Atom& atom, const std::string& description) {
    std::cout << description << ":\n"
              << "  Element: " << atom.getChemicalElement() << "\n"
              << "  Coordinates: (" << std::fixed << std::setprecision(3)
              << atom.getX() << ", " << atom.getY() << ", " << atom.getZ() << ")\n"
              << "  Atomic Radius: " << atom.getAtomicRadius() << " Å\n"
              << "  Atomic Mass: " << atom.getAtomicMass() << " Da\n\n";
}

int main() {
    std::cout << "=== BioMesh Enhanced Atom Architecture Demo ===\n\n";

    // Demonstrate different Atom constructors
    std::cout << "1. Testing Different Atom Constructors:\n";
    std::cout << "----------------------------------------\n";

    // Constructor with only chemical element
    Atom hydrogen("H");
    printAtom(hydrogen, "Hydrogen atom (element only)");

    // Constructor with chemical element and radius
    Atom carbon("C", 1.70);
    printAtom(carbon, "Carbon atom (element + radius)");

    // Constructor with chemical element, radius, and mass
    Atom nitrogen("N", 1.55, 14.007);
    printAtom(nitrogen, "Nitrogen atom (element + radius + mass)");

    // Traditional constructor with coordinates
    Atom oxygen(10.5, 20.3, 15.7, "O");
    printAtom(oxygen, "Oxygen atom (coordinates + element)");

    std::cout << "2. Demonstrating AtomBuilder Pattern:\n";
    std::cout << "------------------------------------\n";

    // Create a vector of parsed atoms (as if from a PDB file)
    std::vector<Atom> parsedAtoms;
    parsedAtoms.emplace_back(1.234, 5.678, 9.012, "C");    // Carbon
    parsedAtoms.emplace_back(2.345, 6.789, 0.123, "N");    // Nitrogen
    parsedAtoms.emplace_back(3.456, 7.890, 1.234, "O");    // Oxygen
    parsedAtoms.emplace_back(4.567, 8.901, 2.345, "P");    // Phosphorus
    parsedAtoms.emplace_back(5.678, 9.012, 3.456, "S");    // Sulfur
    parsedAtoms.emplace_back(6.789, 0.123, 4.567, "H");    // Hydrogen

    std::cout << "Parsed atoms (coordinates + elements only):\n";
    for (size_t i = 0; i < parsedAtoms.size(); ++i) {
        printAtom(parsedAtoms[i], "Parsed atom " + std::to_string(i + 1));
    }

    // Use AtomBuilder to enhance atoms with radius and mass
    std::cout << "3. Using AtomBuilder to Enhance Atoms:\n";
    std::cout << "-------------------------------------\n";

    try {
        AtomBuilder builder;
        auto enhancedAtoms = builder.buildAtoms(parsedAtoms);

        std::cout << "Enhanced atoms (with radius and mass assigned):\n";
        for (size_t i = 0; i < enhancedAtoms.size(); ++i) {
            printAtom(enhancedAtoms[i], "Enhanced atom " + std::to_string(i + 1));
        }

        // Demonstrate error handling
        std::cout << "4. Demonstrating Error Handling:\n";
        std::cout << "--------------------------------\n";

        std::vector<Atom> atomsWithUnknownElement;
        atomsWithUnknownElement.emplace_back(1.0, 2.0, 3.0, "UnknownElement");

        try {
            auto failedAtoms = builder.buildAtoms(atomsWithUnknownElement);
        } catch (const std::runtime_error& e) {
            std::cout << "Caught expected error: " << e.what() << "\n\n";
        }

        // Demonstrate adding custom atomic specification
        std::cout << "5. Adding Custom Atomic Specification:\n";
        std::cout << "-------------------------------------\n";

        builder.addAtomicSpec("CustomElement", 2.5, 150.0);
        std::cout << "Added custom element 'CustomElement' with radius 2.5 Å and mass 150.0 Da\n";

        if (builder.hasElement("CustomElement")) {
            const auto& spec = builder.getAtomicSpec("CustomElement");
            std::cout << "Custom element found: " << spec.elementSymbol 
                      << " (radius: " << spec.radius << " Å, mass: " << spec.mass << " Da)\n\n";
        }

        // Test the custom element
        std::vector<Atom> customAtoms;
        customAtoms.emplace_back(0.0, 0.0, 0.0, "CustomElement");
        auto enhancedCustomAtoms = builder.buildAtoms(customAtoms);
        printAtom(enhancedCustomAtoms[0], "Custom element atom");

        std::cout << "6. Summary - Atomic Properties of Common Elements:\n";
        std::cout << "------------------------------------------------\n";
        std::vector<std::string> commonElements = {"H", "C", "N", "O", "P", "S"};
        
        std::cout << std::left << std::setw(8) << "Element" 
                  << std::setw(12) << "Radius (Å)" 
                  << std::setw(12) << "Mass (Da)" << "\n";
        std::cout << std::string(32, '-') << "\n";
        
        for (const auto& element : commonElements) {
            const auto& spec = builder.getAtomicSpec(element);
            std::cout << std::left << std::setw(8) << spec.elementSymbol
                      << std::setw(12) << std::fixed << std::setprecision(3) << spec.radius
                      << std::setw(12) << std::fixed << std::setprecision(3) << spec.mass << "\n";
        }

        // Demonstrate BoundingBox functionality
        std::cout << "\n7. BoundingBox Spatial Analysis:\n";
        std::cout << "-------------------------------\n";

        // Create a BoundingBox and calculate bounds from enhanced atoms
        BoundingBox boundingBox;
        boundingBox.calculateFromAtoms(enhancedAtoms);

        std::cout << "Bounding box calculated from enhanced atoms:\n";
        if (!boundingBox.isEmpty()) {
            std::cout << "  Min coordinates: (" << std::fixed << std::setprecision(3)
                      << boundingBox.getMinX() << ", " << boundingBox.getMinY() << ", " << boundingBox.getMinZ() << ")\n";
            std::cout << "  Max coordinates: (" << std::fixed << std::setprecision(3)
                      << boundingBox.getMaxX() << ", " << boundingBox.getMaxY() << ", " << boundingBox.getMaxZ() << ")\n";
            
            double centerX, centerY, centerZ;
            boundingBox.getCenter(centerX, centerY, centerZ);
            std::cout << "  Center: (" << std::fixed << std::setprecision(3)
                      << centerX << ", " << centerY << ", " << centerZ << ")\n";
            
            std::cout << "  Dimensions - Width: " << std::fixed << std::setprecision(3) << boundingBox.getWidth()
                      << " Å, Height: " << boundingBox.getHeight() 
                      << " Å, Depth: " << boundingBox.getDepth() << " Å\n";
            std::cout << "  Volume: " << std::fixed << std::setprecision(3) << boundingBox.getVolume() << " ų\n";
        } else {
            std::cout << "  Bounding box is empty\n";
        }

        // Test containment
        std::cout << "\n8. Point and Atom Containment Testing:\n";
        std::cout << "-------------------------------------\n";

        // Test some points for containment
        std::vector<std::array<double, 3>> testPoints = {
            {3.0, 7.0, 2.0},  // Should be inside
            {0.0, 0.0, 0.0},  // Should be outside
            {10.0, 10.0, 10.0}  // Should be outside
        };

        for (size_t i = 0; i < testPoints.size(); ++i) {
            const auto& point = testPoints[i];
            bool contained = boundingBox.contains(point[0], point[1], point[2]);
            std::cout << "  Point (" << std::fixed << std::setprecision(1) 
                      << point[0] << ", " << point[1] << ", " << point[2] 
                      << ") is " << (contained ? "inside" : "outside") << " the bounding box\n";
        }

        // Test atom containment
        std::cout << "\nAtom containment testing:\n";
        for (size_t i = 0; i < std::min(size_t(3), enhancedAtoms.size()); ++i) {
            const auto& atom = enhancedAtoms[i];
            bool contained = boundingBox.contains(atom);
            std::cout << "  " << atom.getChemicalElement() << " atom at (" 
                      << std::fixed << std::setprecision(3)
                      << atom.getX() << ", " << atom.getY() << ", " << atom.getZ() 
                      << ") is " << (contained ? "inside" : "outside") << " the bounding box\n";
        }

        // Demonstrate expansion
        std::cout << "\n9. BoundingBox Expansion:\n";
        std::cout << "------------------------\n";
        
        double originalVolume = boundingBox.getVolume();
        std::cout << "Original volume: " << std::fixed << std::setprecision(3) << originalVolume << " ų\n";
        
        boundingBox.expand(1.0);  // Expand by 1 Å in all directions
        std::cout << "After expanding by 1.0 Å:\n";
        std::cout << "  New volume: " << std::fixed << std::setprecision(3) << boundingBox.getVolume() << " ų\n";
        std::cout << "  New dimensions - Width: " << std::fixed << std::setprecision(3) << boundingBox.getWidth()
                  << " Å, Height: " << boundingBox.getHeight() 
                  << " Å, Depth: " << boundingBox.getDepth() << " Å\n";

        // Demonstrate different BoundingBox construction methods
        std::cout << "\n10. BoundingBox Construction Methods:\n";
        std::cout << "------------------------------------\n";

        // Default constructor (empty)
        BoundingBox emptyBox;
        std::cout << "Empty bounding box: " << (emptyBox.isEmpty() ? "empty" : "not empty") 
                  << ", valid: " << (emptyBox.isValid() ? "yes" : "no") << "\n";

        // Explicit bounds constructor
        BoundingBox explicitBox(-5.0, -5.0, -5.0, 5.0, 5.0, 5.0);
        std::cout << "Explicit bounds box (-5,-5,-5) to (5,5,5):\n";
        std::cout << "  Volume: " << std::fixed << std::setprecision(1) << explicitBox.getVolume() << " ų\n";
        std::cout << "  Contains origin (0,0,0): " << (explicitBox.contains(0.0, 0.0, 0.0) ? "yes" : "no") << "\n";

        // Add individual points
        BoundingBox pointBox;
        pointBox.addPoint(1.0, 2.0, 3.0);
        pointBox.addPoint(-1.0, -2.0, -3.0);
        pointBox.addPoint(0.0, 0.0, 0.0);
        
        double centerX, centerY, centerZ;
        pointBox.getCenter(centerX, centerY, centerZ);
        std::cout << "Point-by-point constructed box:\n";
        std::cout << "  Center: (" << std::fixed << std::setprecision(1) 
                  << centerX << ", " << centerY << ", " << centerZ << ")\n";
        std::cout << "  Contains (0.5, 1.0, 1.5): " << (pointBox.contains(0.5, 1.0, 1.5) ? "yes" : "no") << "\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n=== Demo Complete ===\n";
    return 0;
}