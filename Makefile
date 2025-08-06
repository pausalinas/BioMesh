CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2

TARGET = pdb_example
SOURCE = pdb_example.cpp

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)

clean:
	rm -f $(TARGET) sample.pdb

.PHONY: clean