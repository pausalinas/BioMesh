# Makefile for PDB Reader
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
TARGET = pdb_reader
SOURCES = main.cpp pdb_reader.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = pdb_reader.h

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Build object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET)

# Install dependencies (if needed)
install-deps:
	@echo "No external dependencies required for this project."

# Help target
help:
	@echo "Available targets:"
	@echo "  all        - Build the PDB reader (default)"
	@echo "  clean      - Remove build files"
	@echo "  help       - Show this help message"
	@echo ""
	@echo "Usage after building:"
	@echo "  ./$(TARGET) <pdb_file>"

.PHONY: all clean install-deps help