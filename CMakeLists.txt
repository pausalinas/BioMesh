cmake_minimum_required(VERSION 3.16)
project(BioMesh VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.h" "include/*.hpp")

# Create library
add_library(biomesh ${SOURCES} ${HEADERS})
target_include_directories(biomesh PUBLIC include)

# Example executable
add_executable(biomesh_example examples/main.cpp)
target_link_libraries(biomesh_example biomesh)

# Tests (if available)
find_package(GTest QUIET)
if(GTest_FOUND)
    enable_testing()
    file(GLOB_RECURSE TEST_SOURCES "tests/*.cpp")
    add_executable(biomesh_tests ${TEST_SOURCES})
    target_link_libraries(biomesh_tests biomesh GTest::GTest GTest::Main)
    include(GoogleTest)
    gtest_discover_tests(biomesh_tests)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(biomesh PRIVATE /W4)
else()
    target_compile_options(biomesh PRIVATE -Wall -Wextra -Wpedantic)
endif()