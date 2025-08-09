cmake_minimum_required(VERSION 3.10)
project(BioMesh VERSION 1.0.0 LANGUAGES CXX)

# Set C++14 standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Create BioMesh library
set(BIOMESH_SOURCES
    src/Atom.cpp
    src/PDBReader.cpp
    src/BoundingBox.cpp
)

set(BIOMESH_HEADERS
    include/Atom.h
    include/PDBReader.h
    include/BoundingBox.h
)

add_library(biomesh ${BIOMESH_SOURCES} ${BIOMESH_HEADERS})

# Set include directory for the library
target_include_directories(biomesh PUBLIC include)

# Example executable
add_executable(pdb_example examples/pdb_example.cpp)
target_link_libraries(pdb_example biomesh)

# Set compiler warnings
if(MSVC)
    target_compile_options(biomesh PRIVATE /W4)
else()
    target_compile_options(biomesh PRIVATE -Wall -Wextra -Wpedantic)
endif()