cmake_minimum_required(VERSION 3.10)
project(BioMesh)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add compiler flags for better error reporting
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

# Add executable
add_executable(pdb_example pdb_example.cpp)

# Optional: Add debug flags for development
set(CMAKE_BUILD_TYPE Debug)