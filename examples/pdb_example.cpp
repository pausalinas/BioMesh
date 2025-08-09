#include <iostream>
#include <iomanip>
#include "PDBReader.h"
#include "BoundingBox.h"

using namespace BioMesh;

/**
 * @brief Create a sample PDB content for demonstration
 * 
 * @return std::string Sample PDB content with some atoms
 */
std::string createSamplePDB() {
    return R"(HEADER    SAMPLE PROTEIN                          01-JAN-25   DEMO
ATOM      1  N   ALA A   1      20.154  16.967  23.421  1.00 20.00           N  
ATOM      2  CA  ALA A   1      19.030  16.101  23.938  1.00 20.00           C  
ATOM      3  C   ALA A   1      18.612  16.619  25.293  1.00 20.00           C  
ATOM      4  O   ALA A   1      17.506  17.095  25.456  1.00 20.00           O  
ATOM      5  CB  ALA A   1      17.829  16.101  23.021  1.00 20.00           C  
ATOM      6  N   GLY A   2      19.456  16.562  26.320  1.00 20.00           N  
ATOM      7  CA  GLY A   2      19.175  17.018  27.672  1.00 20.00           C  
ATOM      8  C   GLY A   2      18.058  16.345  28.436  1.00 20.00           C  
ATOM      9  O   GLY A   2      17.289  16.910  29.187  1.00 20.00           O  
ATOM     10  N   VAL A   3      17.974  15.030  28.294  1.00 20.00           N  
END)";
}

/**
 * @brief Demonstrate basic PDBReader functionality
 * 
 * @param reader PDBReader instance
 */
void demonstratePDBReader(PDBReader& reader) {
    std::cout << "=== PDBReader Demonstration ===" << std::endl;
    
    // Parse sample PDB content
    std::string samplePDB = createSamplePDB();
    bool success = reader.parseContent(samplePDB);
    
    if (!success) {
        std::cout << "Failed to parse PDB content!" << std::endl;
        return;
    }
    
    std::cout << "Successfully parsed " << reader.getAtomCount() << " atoms" << std::endl;
    std::cout << std::endl;
    
    // Display first few atoms
    const auto& atoms = reader.getAtoms();
    std::cout << "First 5 atoms:" << std::endl;
    std::cout << std::setw(5) << "Index" 
              << std::setw(8) << "Name"
              << std::setw(8) << "Element"
              << std::setw(10) << "X"
              << std::setw(10) << "Y"
              << std::setw(10) << "Z" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    for (size_t i = 0; i < std::min(size_t(5), atoms.size()); ++i) {
        const auto& atom = atoms[i];
        std::cout << std::setw(5) << i + 1
                  << std::setw(8) << atom.getName()
                  << std::setw(8) << atom.getElement()
                  << std::setw(10) << std::fixed << std::setprecision(3) << atom.getX()
                  << std::setw(10) << std::fixed << std::setprecision(3) << atom.getY()
                  << std::setw(10) << std::fixed << std::setprecision(3) << atom.getZ()
                  << std::endl;
    }
    std::cout << std::endl;
}

/**
 * @brief Demonstrate BoundingBox functionality
 * 
 * @param atoms Vector of atoms to work with
 */
void demonstrateBoundingBox(const std::vector<Atom>& atoms) {
    std::cout << "=== BoundingBox Demonstration ===" << std::endl;
    
    // Create bounding box and calculate from atoms
    BoundingBox bbox;
    bbox.calculateFromAtoms(atoms);
    
    if (bbox.isEmpty()) {
        std::cout << "No atoms to calculate bounding box from!" << std::endl;
        return;
    }
    
    std::cout << "Bounding box calculated from " << atoms.size() << " atoms:" << std::endl;
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "  Min coordinates: (" << bbox.getMinX() << ", " 
              << bbox.getMinY() << ", " << bbox.getMinZ() << ")" << std::endl;
    std::cout << "  Max coordinates: (" << bbox.getMaxX() << ", " 
              << bbox.getMaxY() << ", " << bbox.getMaxZ() << ")" << std::endl;
    
    std::cout << "  Dimensions:" << std::endl;
    std::cout << "    Width  (X): " << bbox.getWidth() << std::endl;
    std::cout << "    Height (Y): " << bbox.getHeight() << std::endl;
    std::cout << "    Depth  (Z): " << bbox.getDepth() << std::endl;
    std::cout << "    Volume:     " << bbox.getVolume() << std::endl;
    
    double centerX, centerY, centerZ;
    bbox.getCenter(centerX, centerY, centerZ);
    std::cout << "  Center: (" << centerX << ", " << centerY << ", " << centerZ << ")" << std::endl;
    
    // Demonstrate point containment
    std::cout << std::endl << "Point containment tests:" << std::endl;
    
    // Test center point (should be inside)
    bool centerInside = bbox.contains(centerX, centerY, centerZ);
    std::cout << "  Center point (" << centerX << ", " << centerY << ", " << centerZ 
              << ") is " << (centerInside ? "inside" : "outside") << std::endl;
    
    // Test a point outside
    double outsideX = bbox.getMaxX() + 10.0;
    double outsideY = bbox.getMaxY() + 10.0;
    double outsideZ = bbox.getMaxZ() + 10.0;
    bool outsidePoint = bbox.contains(outsideX, outsideY, outsideZ);
    std::cout << "  Point (" << outsideX << ", " << outsideY << ", " << outsideZ 
              << ") is " << (outsidePoint ? "inside" : "outside") << std::endl;
    
    // Test atom containment
    if (!atoms.empty()) {
        bool firstAtomInside = bbox.contains(atoms[0]);
        std::cout << "  First atom is " << (firstAtomInside ? "inside" : "outside") << std::endl;
    }
    
    // Demonstrate expansion
    std::cout << std::endl << "Expanding bounding box by 2.0 units..." << std::endl;
    bbox.expand(2.0);
    std::cout << "  New min coordinates: (" << bbox.getMinX() << ", " 
              << bbox.getMinY() << ", " << bbox.getMinZ() << ")" << std::endl;
    std::cout << "  New max coordinates: (" << bbox.getMaxX() << ", " 
              << bbox.getMaxY() << ", " << bbox.getMaxZ() << ")" << std::endl;
    std::cout << "  New volume: " << bbox.getVolume() << std::endl;
    std::cout << std::endl;
}

/**
 * @brief Demonstrate manual bounding box operations
 */
void demonstrateManualBoundingBox() {
    std::cout << "=== Manual BoundingBox Operations ===" << std::endl;
    
    // Create empty bounding box and add points manually
    BoundingBox bbox;
    std::cout << "Created empty bounding box. Is empty: " << (bbox.isEmpty() ? "yes" : "no") << std::endl;
    
    // Add some points
    bbox.addPoint(0.0, 0.0, 0.0);
    bbox.addPoint(10.0, 5.0, 8.0);
    bbox.addPoint(-2.0, 3.0, 1.0);
    
    std::cout << "Added 3 points. Is empty: " << (bbox.isEmpty() ? "yes" : "no") << std::endl;
    std::cout << "Is valid: " << (bbox.isValid() ? "yes" : "no") << std::endl;
    
    std::cout << std::fixed << std::setprecision(3);
    std::cout << "Final bounding box:" << std::endl;
    std::cout << "  Min: (" << bbox.getMinX() << ", " << bbox.getMinY() << ", " << bbox.getMinZ() << ")" << std::endl;
    std::cout << "  Max: (" << bbox.getMaxX() << ", " << bbox.getMaxY() << ", " << bbox.getMaxZ() << ")" << std::endl;
    std::cout << "  Dimensions: " << bbox.getWidth() << " x " << bbox.getHeight() << " x " << bbox.getDepth() << std::endl;
    
    // Reset and verify
    bbox.reset();
    std::cout << "After reset - Is empty: " << (bbox.isEmpty() ? "yes" : "no") << std::endl;
    std::cout << std::endl;
}

int main() {
    std::cout << "BioMesh PDB Example - Demonstrating BoundingBox with Atom Data" << std::endl;
    std::cout << std::string(65, '=') << std::endl;
    std::cout << std::endl;
    
    try {
        // Create PDB reader and parse sample data
        PDBReader reader;
        demonstratePDBReader(reader);
        
        // Demonstrate BoundingBox with the parsed atoms
        if (reader.hasAtoms()) {
            demonstrateBoundingBox(reader.getAtoms());
        }
        
        // Demonstrate manual BoundingBox operations
        demonstrateManualBoundingBox();
        
        std::cout << "Example completed successfully!" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}