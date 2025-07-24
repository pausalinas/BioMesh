# BioMesh PDB Parser Makefile
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
DEBUG_FLAGS = -std=c++11 -Wall -Wextra -g -DDEBUG
TARGET = pdb_parser
SOURCES = main.cpp pdb_parser.cpp
HEADERS = pdb_parser.h
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Run with: ./$(TARGET) <pdb_file>"

# Compile individual object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS = $(DEBUG_FLAGS)
debug: clean $(TARGET)
	@echo "Debug build complete!"

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete!"

# Install (optional - copies to /usr/local/bin)
install: $(TARGET)
	sudo cp $(TARGET) /usr/local/bin/
	@echo "Installed to /usr/local/bin/"

# Uninstall
uninstall:
	sudo rm -f /usr/local/bin/$(TARGET)
	@echo "Uninstalled from /usr/local/bin/"

# Test with a sample PDB file (requires sample.pdb)
test: $(TARGET)
	@if [ -f "sample.pdb" ]; then \
		echo "Testing with sample.pdb..."; \
		./$(TARGET) sample.pdb; \
	else \
		echo "No sample.pdb file found. Please provide a PDB file to test."; \
		echo "Usage: ./$(TARGET) <your_pdb_file>"; \
	fi

# Help
help:
	@echo "BioMesh PDB Parser Build System"
	@echo "==============================="
	@echo "Available targets:"
	@echo "  all      - Build the parser (default)"
	@echo "  debug    - Build with debug symbols"
	@echo "  clean    - Remove build artifacts"
	@echo "  install  - Install to /usr/local/bin (requires sudo)"
	@echo "  uninstall- Remove from /usr/local/bin (requires sudo)"
	@echo "  test     - Test with sample.pdb (if available)"
	@echo "  help     - Show this help message"
	@echo ""
	@echo "Usage examples:"
	@echo "  make              # Build the parser"
	@echo "  make debug        # Build with debug info"
	@echo "  ./pdb_parser protein.pdb  # Run the parser"

# Phony targets
.PHONY: all debug clean install uninstall test help