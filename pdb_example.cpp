#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <memory>
#include <stdexcept>

/**
 * Structure to represent an atom from a PDB file
 */
struct Atom {
    int serial;
    std::string name;
    std::string resName;
    char chainID;
    int resSeq;
    double x, y, z;
    double occupancy;
    double tempFactor;
    std::string element;
    
    Atom() : serial(0), chainID(' '), resSeq(0), x(0.0), y(0.0), z(0.0), 
             occupancy(0.0), tempFactor(0.0) {}
};

/**
 * RAII-based PDB file parser
 */
class PDBParser {
private:
    std::vector<Atom> atoms_;
    std::string filename_;
    
    /**
     * Parse a single ATOM line from PDB file
     */
    bool parseAtomLine(const std::string& line, Atom& atom) {
        if (line.length() < 54) return false;
        
        try {
            atom.serial = std::stoi(line.substr(6, 5));
            atom.name = line.substr(12, 4);
            atom.resName = line.substr(17, 3);
            atom.chainID = line.length() > 21 ? line[21] : ' ';
            atom.resSeq = std::stoi(line.substr(22, 4));
            atom.x = std::stod(line.substr(30, 8));
            atom.y = std::stod(line.substr(38, 8));
            atom.z = std::stod(line.substr(46, 8));
            
            if (line.length() >= 60) {
                atom.occupancy = std::stod(line.substr(54, 6));
            }
            if (line.length() >= 66) {
                atom.tempFactor = std::stod(line.substr(60, 6));
            }
            if (line.length() >= 78) {
                atom.element = line.substr(76, 2);
                // Trim whitespace
                atom.element.erase(atom.element.find_last_not_of(" \t") + 1);
            }
            
            // Trim whitespace from string fields
            atom.name.erase(atom.name.find_last_not_of(" \t") + 1);
            atom.resName.erase(atom.resName.find_last_not_of(" \t") + 1);
            
            return true;
        } catch (const std::exception&) {
            return false;
        }
    }
    
public:
    /**
     * Constructor with filename - follows RAII principles
     */
    explicit PDBParser(const std::string& filename) : filename_(filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open PDB file: " + filename);
        }
        
        std::string line;
        while (std::getline(file, line)) {
            if (line.substr(0, 4) == "ATOM" || line.substr(0, 6) == "HETATM") {
                Atom atom;
                if (parseAtomLine(line, atom)) {
                    atoms_.push_back(atom);
                }
            }
        }
        
        if (atoms_.empty()) {
            throw std::runtime_error("No valid atoms found in PDB file: " + filename);
        }
    }
    
    /**
     * Get total number of atoms
     */
    size_t getTotalAtoms() const {
        return atoms_.size();
    }
    
    /**
     * Get reference to all atoms
     */
    const std::vector<Atom>& getAtoms() const {
        return atoms_;
    }
    
    /**
     * Display results with enhanced formatting
     */
    void displayResults() const {
        const size_t totalAtoms = atoms_.size();
        
        // Display total atom count prominently
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "           PDB PARSING RESULTS" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        std::cout << "File: " << filename_ << std::endl;
        std::cout << "Total atoms parsed: " << totalAtoms << std::endl;
        std::cout << std::string(60, '-') << std::endl;
        
        // Display first 5 atoms
        std::cout << "\nFirst 5 atoms:" << std::endl;
        displayAtomHeader();
        const size_t firstCount = std::min(static_cast<size_t>(5), totalAtoms);
        for (size_t i = 0; i < firstCount; ++i) {
            displayAtom(atoms_[i], i + 1);
        }
        
        // Display separator if more than 10 atoms total
        if (totalAtoms > 10) {
            std::cout << "\n" << std::string(60, '.') << std::endl;
            std::cout << "... " << (totalAtoms - 10) << " atoms in between ..." << std::endl;
            std::cout << std::string(60, '.') << std::endl;
        }
        
        // Display last 5 atoms (if more than 5 total)
        if (totalAtoms > 5) {
            std::cout << "\nLast 5 atoms:" << std::endl;
            displayAtomHeader();
            const size_t lastStart = totalAtoms > 10 ? totalAtoms - 5 : 5;
            for (size_t i = lastStart; i < totalAtoms; ++i) {
                displayAtom(atoms_[i], i + 1);
            }
        }
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
    }
    
private:
    /**
     * Display header for atom information
     */
    void displayAtomHeader() const {
        std::cout << std::left 
                  << std::setw(4) << "#"
                  << std::setw(6) << "Serial"
                  << std::setw(5) << "Name"
                  << std::setw(4) << "Res"
                  << std::setw(2) << "Ch"
                  << std::setw(5) << "ResN"
                  << std::setw(10) << "X"
                  << std::setw(10) << "Y"
                  << std::setw(10) << "Z"
                  << std::setw(4) << "Elm" << std::endl;
        std::cout << std::string(60, '-') << std::endl;
    }
    
    /**
     * Display a single atom with improved coordinate formatting
     */
    void displayAtom(const Atom& atom, size_t index) const {
        std::cout << std::left << std::setw(4) << index
                  << std::setw(6) << atom.serial
                  << std::setw(5) << atom.name
                  << std::setw(4) << atom.resName
                  << std::setw(2) << atom.chainID
                  << std::setw(5) << atom.resSeq
                  << std::right << std::fixed << std::setprecision(2)
                  << std::setw(10) << atom.x
                  << std::setw(10) << atom.y
                  << std::setw(10) << atom.z
                  << std::left << std::setw(4) << atom.element
                  << std::endl;
    }
};

/**
 * Create a sample PDB file for testing
 */
void createSamplePDB(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot create sample PDB file: " + filename);
    }
    
    // Sample PDB data with 15 atoms for testing first/last display
    file << "ATOM      1  N   ALA A   1      20.154  16.967  27.462  1.00 11.18           N  \n";
    file << "ATOM      2  CA  ALA A   1      19.030  16.099  27.090  1.00 10.90           C  \n";
    file << "ATOM      3  C   ALA A   1      18.462  16.632  25.788  1.00 10.74           C  \n";
    file << "ATOM      4  O   ALA A   1      18.820  17.689  25.268  1.00 11.01           O  \n";
    file << "ATOM      5  CB  ALA A   1      17.955  16.012  28.176  1.00 10.71           C  \n";
    file << "ATOM      6  N   THR A   2      17.401  15.957  25.373  1.00 10.38           N  \n";
    file << "ATOM      7  CA  THR A   2      16.745  16.324  24.124  1.00 10.14           C  \n";
    file << "ATOM      8  C   THR A   2      15.395  15.606  23.982  1.00  9.85           C  \n";
    file << "ATOM      9  O   THR A   2      15.078  14.750  24.800  1.00  9.83           O  \n";
    file << "ATOM     10  CB  THR A   2      17.527  16.065  22.828  1.00 10.24           C  \n";
    file << "ATOM     11  OG1 THR A   2      18.682  16.903  22.807  1.00 10.47           O  \n";
    file << "ATOM     12  CG2 THR A   2      16.719  16.342  21.567  1.00 10.32           C  \n";
    file << "ATOM     13  N   VAL A   3      14.692  15.839  22.876  1.00  9.55           N  \n";
    file << "ATOM     14  CA  VAL A   3      13.390  15.180  22.616  1.00  9.35           C  \n";
    file << "ATOM     15  C   VAL A   3      12.633  15.795  21.451  1.00  9.19           C  \n";
    
    file.close();
}

int main(int argc, char* argv[]) {
    try {
        std::string filename;
        
        if (argc > 1) {
            filename = argv[1];
        } else {
            // Create a sample PDB file for demonstration
            filename = "sample.pdb";
            std::cout << "No PDB file specified. Creating sample file: " << filename << std::endl;
            createSamplePDB(filename);
        }
        
        // Parse PDB file using RAII principles
        PDBParser parser(filename);
        
        // Display results with enhanced formatting
        parser.displayResults();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        std::cerr << "Usage: " << (argc > 0 ? argv[0] : "pdb_example") 
                  << " [pdb_file]" << std::endl;
        return 1;
    }
    
    return 0;
}