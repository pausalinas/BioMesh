cmake_minimum_required(VERSION 3.10)
project(BioMesh)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add include directory
include_directories(include)

# Create the main library
add_library(biomesh_lib 
    src/PDBReader.cpp
)

# Create the pdb_viewer example
add_executable(pdb_viewer
    examples/pdb_viewer.cpp
)
target_link_libraries(pdb_viewer biomesh_lib)

# Create test data generator
add_executable(test_generator
    tests/test_generator.cpp
)
target_link_libraries(test_generator biomesh_lib)

# Enable testing
enable_testing()

# Create unit tests
add_executable(unit_tests
    tests/unit_tests.cpp
)
target_link_libraries(unit_tests biomesh_lib)
add_test(NAME UnitTests COMMAND unit_tests)

# Create integration tests
add_executable(integration_tests
    tests/integration_tests.cpp
)
target_link_libraries(integration_tests biomesh_lib)
add_test(NAME IntegrationTests COMMAND integration_tests)