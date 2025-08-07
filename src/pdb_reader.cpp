#include "biomesh/pdb_reader.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace biomesh {

PDBReader::PDBReader(std::shared_ptr<AtomicSpecTable> spec_table)
    : spec_table_(spec_table) {
    if (!spec_table_) {
        spec_table_ = std::make_shared<AtomicSpecTable>();
    }
}

void PDBReader::readFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw PDBFileException("Cannot open file: " + filename);
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }
    file.close();

    parseContent(content);
}

void PDBReader::parseContent(const std::string& content) {
    clear();

    std::istringstream stream(content);
    std::string line;
    int line_number = 0;

    while (std::getline(stream, line)) {
        ++line_number;
        
        // Skip empty lines and non-ATOM records
        if (line.length() < 6) continue;
        
        std::string record_type = line.substr(0, 6);
        if (record_type == "ATOM  " || record_type == "HETATM") {
            try {
                Atom atom = parseAtomLine(line);
                
                // Add to appropriate molecule
                Molecule& molecule = getOrCreateMolecule(atom.chain_id);
                molecule.addAtom(atom);
                
                // Add to flat list for backward compatibility
                all_atoms_.push_back(std::move(atom));
                
            } catch (const PDBParseException& e) {
                throw PDBParseException("Line " + std::to_string(line_number) + ": " + e.what());
            }
        }
    }

    if (all_atoms_.empty()) {
        throw PDBParseException("No valid ATOM records found in PDB content");
    }
}

const Molecule* PDBReader::getMolecule(const std::string& chain_id) const {
    auto it = molecules_.find(chain_id);
    return (it != molecules_.end()) ? it->second.get() : nullptr;
}

void PDBReader::clear() {
    molecules_.clear();
    all_atoms_.clear();
}

std::vector<std::string> PDBReader::getChainIds() const {
    std::vector<std::string> chain_ids;
    chain_ids.reserve(molecules_.size());
    
    for (const auto& pair : molecules_) {
        chain_ids.push_back(pair.first);
    }
    
    std::sort(chain_ids.begin(), chain_ids.end());
    return chain_ids;
}

void PDBReader::setAtomicSpecTable(std::shared_ptr<AtomicSpecTable> spec_table) {
    spec_table_ = spec_table;
    if (!spec_table_) {
        spec_table_ = std::make_shared<AtomicSpecTable>();
    }
}

Atom PDBReader::parseAtomLine(const std::string& line) {
    if (line.length() < 54) {
        throw PDBParseException("ATOM line too short (minimum 54 characters required)");
    }

    try {
        // Parse according to PDB format specification
        int serial = parseInt(extractField(line, 6, 5), "serial number");
        std::string atom_name = extractField(line, 12, 4);
        std::string residue_name = extractField(line, 17, 3);
        std::string chain_id = extractField(line, 21, 1);
        int residue_number = parseInt(extractField(line, 22, 4), "residue number");
        
        double x = parseDouble(extractField(line, 30, 8), "X coordinate");
        double y = parseDouble(extractField(line, 38, 8), "Y coordinate");
        double z = parseDouble(extractField(line, 46, 8), "Z coordinate");

        // Use AtomBuilder to construct atom with chemical properties
        AtomBuilder builder(spec_table_);
        return builder.setSerial(serial)
                     .setAtomName(atom_name)
                     .setResidueName(residue_name)
                     .setChainId(chain_id)
                     .setResidueNumber(residue_number)
                     .setCoordinates(x, y, z)
                     .autoDetectElement()
                     .build();
                     
    } catch (const std::exception& e) {
        throw PDBParseException("Failed to parse ATOM line: " + std::string(e.what()));
    }
}

Molecule& PDBReader::getOrCreateMolecule(const std::string& chain_id) {
    auto it = molecules_.find(chain_id);
    if (it == molecules_.end()) {
        auto result = molecules_.emplace(chain_id, std::make_unique<Molecule>(chain_id));
        return *result.first->second;
    }
    return *it->second;
}

std::string PDBReader::trim(const std::string& str) const {
    size_t start = str.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) {
        return "";
    }
    size_t end = str.find_last_not_of(" \t\r\n");
    return str.substr(start, end - start + 1);
}

std::string PDBReader::extractField(const std::string& line, size_t start, size_t length) const {
    if (start >= line.length()) {
        return "";
    }
    
    size_t actual_length = std::min(length, line.length() - start);
    std::string field = line.substr(start, actual_length);
    return trim(field);
}

int PDBReader::parseInt(const std::string& str, const std::string& field_name) const {
    if (str.empty()) {
        throw PDBParseException("Empty " + field_name + " field");
    }
    
    try {
        return std::stoi(str);
    } catch (const std::exception&) {
        throw PDBParseException("Invalid " + field_name + ": '" + str + "'");
    }
}

double PDBReader::parseDouble(const std::string& str, const std::string& field_name) const {
    if (str.empty()) {
        throw PDBParseException("Empty " + field_name + " field");
    }
    
    try {
        return std::stod(str);
    } catch (const std::exception&) {
        throw PDBParseException("Invalid " + field_name + ": '" + str + "'");
    }
}

} // namespace biomesh