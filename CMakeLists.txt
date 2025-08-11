cmake_minimum_required(VERSION 3.10)
project(BioMesh VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Find required packages
find_package(Threads REQUIRED)

# Source files
set(SOURCES
    src/Atom.cpp
    src/AtomBuilder.cpp
    src/BoundingBox.cpp
)

# Create library
add_library(biomesh ${SOURCES})
target_include_directories(biomesh PUBLIC include)

# Examples
add_executable(atom_example examples/atom_example.cpp)
target_link_libraries(atom_example biomesh)

# Enable testing
enable_testing()

# Try to find GoogleTest system package
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(GTEST gtest)
    pkg_check_modules(GTEST_MAIN gtest_main)
endif()

# Check if we have GoogleTest available
if(GTEST_FOUND AND GTEST_MAIN_FOUND)
    # Test executable using system GoogleTest
    add_executable(atom_tests tests/atom_tests.cpp)
    target_link_libraries(atom_tests biomesh ${GTEST_LIBRARIES} ${GTEST_MAIN_LIBRARIES} Threads::Threads)
    target_include_directories(atom_tests PRIVATE ${GTEST_INCLUDE_DIRS})
    target_compile_options(atom_tests PRIVATE ${GTEST_CFLAGS_OTHER})
    
    # Add test
    add_test(NAME AtomTests COMMAND atom_tests)
    message(STATUS "GoogleTest found. Tests will be built.")
else()
    # Try manual linkage for Ubuntu package
    find_library(GTEST_LIB gtest PATHS /usr/lib /usr/local/lib)
    find_library(GTEST_MAIN_LIB gtest_main PATHS /usr/lib /usr/local/lib)
    find_path(GTEST_INCLUDE_DIR gtest/gtest.h PATHS /usr/include /usr/local/include)
    
    if(GTEST_LIB AND GTEST_MAIN_LIB AND GTEST_INCLUDE_DIR)
        add_executable(atom_tests tests/atom_tests.cpp)
        target_link_libraries(atom_tests biomesh ${GTEST_LIB} ${GTEST_MAIN_LIB} Threads::Threads)
        target_include_directories(atom_tests PRIVATE ${GTEST_INCLUDE_DIR})
        
        add_test(NAME AtomTests COMMAND atom_tests)
        message(STATUS "GoogleTest found manually. Tests will be built.")
    else()
        message(STATUS "GoogleTest not found. Tests will not be built.")
    endif()
endif()