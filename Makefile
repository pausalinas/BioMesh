# Makefile for BioMesh PDB Reader
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = pdb_reader
SOURCES = main.cpp pdb_reader.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = pdb_reader.h

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Run with: ./$(TARGET) <pdb_file>"

# Build object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

# Install (optional - just copy to /usr/local/bin)
install: $(TARGET)
	cp $(TARGET) /usr/local/bin/
	@echo "Installed $(TARGET) to /usr/local/bin/"

# Uninstall
uninstall:
	rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstalled $(TARGET) from /usr/local/bin/"

# Create a sample PDB file for testing
sample:
	@echo "Creating sample.pdb for testing..."
	@echo "HEADER    SAMPLE PROTEIN                          01-JAN-24   XXXX" > sample.pdb
	@echo "ATOM      1  N   ALA A   1      20.154  16.967  10.568  1.00 20.00           N" >> sample.pdb
	@echo "ATOM      2  CA  ALA A   1      19.030  16.807   9.528  1.00 20.00           C" >> sample.pdb
	@echo "ATOM      3  C   ALA A   1      17.715  17.354  10.121  1.00 20.00           C" >> sample.pdb
	@echo "ATOM      4  O   ALA A   1      17.772  18.342  10.857  1.00 20.00           O" >> sample.pdb
	@echo "ATOM      5  CB  ALA A   1      19.361  17.477   8.189  1.00 20.00           C" >> sample.pdb
	@echo "ATOM      6  N   VAL A   2      16.565  16.827   9.830  1.00 20.00           N" >> sample.pdb
	@echo "ATOM      7  CA  VAL A   2      15.252  17.248  10.287  1.00 20.00           C" >> sample.pdb
	@echo "ATOM      8  C   VAL A   2      14.173  16.178  10.132  1.00 20.00           C" >> sample.pdb
	@echo "ATOM      9  O   VAL A   2      14.089  15.411   9.165  1.00 20.00           O" >> sample.pdb
	@echo "ATOM     10  CB  VAL A   2      14.751  18.527   9.594  1.00 20.00           C" >> sample.pdb
	@echo "END" >> sample.pdb
	@echo "Sample PDB file created: sample.pdb"

# Help target
help:
	@echo "BioMesh PDB Reader Makefile"
	@echo "Available targets:"
	@echo "  all      - Build the program (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  sample   - Create a sample PDB file for testing"
	@echo "  install  - Install to /usr/local/bin (requires sudo)"
	@echo "  uninstall- Remove from /usr/local/bin (requires sudo)"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Manual compilation:"
	@echo "  g++ -std=c++11 -Wall -Wextra -O2 -o pdb_reader main.cpp pdb_reader.cpp"
	@echo ""
	@echo "Usage:"
	@echo "  ./pdb_reader <pdb_file>"

# Declare phony targets
.PHONY: all clean sample install uninstall help