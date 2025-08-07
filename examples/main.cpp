#include "biomesh/biomesh.h"
#include <iostream>
#include <iomanip>

using namespace biomesh;

int main() {
    try {
        std::cout << "BioMesh Demo - Version " << getVersion() << std::endl;
        std::cout << "====================================" << std::endl;

        // Create a PDB reader with default atomic specifications
        auto spec_table = createDefaultSpecTable();
        PDBReader reader(spec_table);

        // Create some sample PDB content for demonstration
        std::string sample_pdb = 
            "ATOM      1  N   ALA A   1      20.154  20.000  20.000  1.00 20.00           N  \n"
            "ATOM      2  CA  ALA A   1      21.155  20.500  20.500  1.00 20.00           C  \n"
            "ATOM      3  C   ALA A   1      22.156  21.000  21.000  1.00 20.00           C  \n"
            "ATOM      4  O   ALA A   1      23.157  21.500  21.500  1.00 20.00           O  \n"
            "ATOM      5  N   GLY B   1      30.000  30.000  30.000  1.00 20.00           N  \n"
            "ATOM      6  CA  GLY B   1      31.000  30.500  30.500  1.00 20.00           C  \n"
            "ATOM      7  C   GLY B   1      32.000  31.000  31.000  1.00 20.00           C  \n"
            "ATOM      8  O   GLY B   1      33.000  31.500  31.500  1.00 20.00           O  \n"
            "HETATM    9  FE  HEM C   1      40.000  40.000  40.000  1.00 30.00          FE  \n";

        std::cout << "Parsing sample PDB content..." << std::endl;
        reader.parseContent(sample_pdb);

        // Display overall statistics
        std::cout << "\nParsing Results:" << std::endl;
        std::cout << "Total molecules: " << reader.getMoleculeCount() << std::endl;
        std::cout << "Total atoms: " << reader.getTotalAtomCount() << std::endl;

        // Display chain information
        std::cout << "\nChain Information:" << std::endl;
        auto chain_ids = reader.getChainIds();
        for (const auto& chain_id : chain_ids) {
            const auto* molecule = reader.getMolecule(chain_id);
            if (molecule) {
                std::cout << "Chain " << chain_id << ": " << molecule->getAtomCount() << " atoms" << std::endl;
                
                // Display bounding box
                const auto& bbox = molecule->getBoundingBox();
                std::cout << "  Bounding box: (" 
                         << std::fixed << std::setprecision(2)
                         << bbox.min_x << ", " << bbox.min_y << ", " << bbox.min_z 
                         << ") to (" 
                         << bbox.max_x << ", " << bbox.max_y << ", " << bbox.max_z << ")" << std::endl;
                
                // Display center and radius
                double cx, cy, cz;
                molecule->getCenter(cx, cy, cz);
                std::cout << "  Center: (" << cx << ", " << cy << ", " << cz << ")" << std::endl;
                std::cout << "  Bounding sphere radius: " << molecule->getBoundingSphereRadius() << std::endl;
            }
        }

        // Display detailed atom information for first molecule
        std::cout << "\nDetailed Atom Information (Chain A):" << std::endl;
        const auto* chain_a = reader.getMolecule("A");
        if (chain_a) {
            const auto& atoms = chain_a->getAtoms();
            for (const auto& atom : atoms) {
                std::cout << "  " << atom.atom_name 
                         << " (" << atom.chemical_element << ")"
                         << " - Radius: " << atom.atomic_radius << "Å"
                         << ", Mass: " << atom.atomic_mass << "u"
                         << ", Coords: (" << atom.x << ", " << atom.y << ", " << atom.z << ")"
                         << std::endl;
            }
        }

        // Demonstrate AtomBuilder usage
        std::cout << "\nDemonstrating AtomBuilder:" << std::endl;
        AtomBuilder builder(spec_table);
        
        Atom custom_atom = builder.reset()
                                  .setSerial(100)
                                  .setAtomName("CA")
                                  .setResidueName("LEU")
                                  .setChainId("X")
                                  .setResidueNumber(10)
                                  .setCoordinates(15.0, 25.0, 35.0)
                                  .autoDetectElement()
                                  .build();
        
        std::cout << "Built atom: " << custom_atom.atom_name 
                 << " (" << custom_atom.chemical_element << ")"
                 << " - Radius: " << custom_atom.atomic_radius << "Å"
                 << ", Mass: " << custom_atom.atomic_mass << "u" << std::endl;

        // Demonstrate specification table access
        std::cout << "\nAtomic Specification Table:" << std::endl;
        std::cout << "Available elements: ";
        std::vector<std::string> elements = {"C", "N", "O", "S", "P", "H", "FE", "ZN"};
        for (const auto& element : elements) {
            auto spec = spec_table->getSpec(element);
            if (spec.has_value()) {
                std::cout << element << "(" << spec->atomic_radius << "Å) ";
            }
        }
        std::cout << std::endl;

        std::cout << "\nDemo completed successfully!" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}