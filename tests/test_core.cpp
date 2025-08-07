#include "biomesh/biomesh.h"
#include <cassert>
#include <iostream>

using namespace biomesh;

void testAtomicSpecTable() {
    std::cout << "Testing AtomicSpecTable..." << std::endl;
    
    AtomicSpecTable table;
    
    // Test default elements
    auto carbon_spec = table.getSpec("C");
    assert(carbon_spec.has_value());
    assert(carbon_spec->element_symbol == "C");
    assert(carbon_spec->atomic_radius == 1.7);
    assert(carbon_spec->atomic_mass == 12.01);
    
    // Test custom element
    table.addSpec(AtomicSpec("X", 2.0, 100.0));
    auto custom_spec = table.getSpec("X");
    assert(custom_spec.has_value());
    assert(custom_spec->atomic_radius == 2.0);
    
    std::cout << "AtomicSpecTable tests passed!" << std::endl;
}

void testAtomBuilder() {
    std::cout << "Testing AtomBuilder..." << std::endl;
    
    auto spec_table = std::make_shared<AtomicSpecTable>();
    AtomBuilder builder(spec_table);
    
    Atom atom = builder.setSerial(1)
                       .setAtomName("CA")
                       .setResidueName("ALA")
                       .setChainId("A")
                       .setResidueNumber(1)
                       .setCoordinates(1.0, 2.0, 3.0)
                       .autoDetectElement()
                       .build();
    
    assert(atom.serial_number == 1);
    assert(atom.atom_name == "CA");
    assert(atom.chemical_element == "C");
    assert(atom.atomic_radius == 1.7);
    assert(atom.x == 1.0 && atom.y == 2.0 && atom.z == 3.0);
    
    std::cout << "AtomBuilder tests passed!" << std::endl;
}

void testMolecule() {
    std::cout << "Testing Molecule..." << std::endl;
    
    Molecule molecule("A");
    assert(molecule.getChainId() == "A");
    assert(molecule.empty());
    
    // Add atoms
    Atom atom1(1, "N", "ALA", "A", 1, 0.0, 0.0, 0.0);
    Atom atom2(2, "CA", "ALA", "A", 1, 1.0, 1.0, 1.0);
    
    molecule.addAtom(atom1);
    molecule.addAtom(atom2);
    
    assert(molecule.getAtomCount() == 2);
    assert(!molecule.empty());
    
    // Test bounding box
    const auto& bbox = molecule.getBoundingBox();
    assert(bbox.min_x == 0.0 && bbox.min_y == 0.0 && bbox.min_z == 0.0);
    assert(bbox.max_x == 1.0 && bbox.max_y == 1.0 && bbox.max_z == 1.0);
    
    double cx, cy, cz;
    molecule.getCenter(cx, cy, cz);
    assert(cx == 0.5 && cy == 0.5 && cz == 0.5);
    
    std::cout << "Molecule tests passed!" << std::endl;
}

void testPDBReader() {
    std::cout << "Testing PDBReader..." << std::endl;
    
    PDBReader reader;
    
    std::string pdb_content = 
        "ATOM      1  N   ALA A   1      10.000  20.000  30.000  1.00 20.00           N  \n"
        "ATOM      2  CA  ALA A   1      11.000  21.000  31.000  1.00 20.00           C  \n"
        "ATOM      3  N   GLY B   1      15.000  25.000  35.000  1.00 20.00           N  \n";
    
    reader.parseContent(pdb_content);
    
    assert(reader.getMoleculeCount() == 2);
    assert(reader.getTotalAtomCount() == 3);
    
    const auto* chain_a = reader.getMolecule("A");
    const auto* chain_b = reader.getMolecule("B");
    
    assert(chain_a != nullptr);
    assert(chain_b != nullptr);
    assert(chain_a->getAtomCount() == 2);
    assert(chain_b->getAtomCount() == 1);
    
    // Test backward compatibility
    const auto& all_atoms = reader.getAllAtoms();
    assert(all_atoms.size() == 3);
    assert(all_atoms[0].chemical_element == "N");
    assert(all_atoms[1].chemical_element == "C");
    
    std::cout << "PDBReader tests passed!" << std::endl;
}

int main() {
    try {
        std::cout << "Running BioMesh Core Tests" << std::endl;
        std::cout << "=========================" << std::endl;
        
        testAtomicSpecTable();
        testAtomBuilder();
        testMolecule();
        testPDBReader();
        
        std::cout << "\nAll tests passed successfully!" << std::endl;
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "Test failed: " << e.what() << std::endl;
        return 1;
    }
}