#include "PDBReader.h"
#include <iostream>

int main() {
    try {
        pdb::PDBReader reader;
        auto atoms = reader.readFile("invalid.pdb");
        std::cout << "Parsed " << atoms.size() << " atoms\n";
    } catch (const pdb::PDBParsingException& e) {
        std::cout << "Parsing error (expected): " << e.what() << std::endl;
        return 0;
    } catch (const pdb::PDBFileException& e) {
        std::cout << "File error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
