cmake_minimum_required(VERSION 3.12)
project(BioMesh VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

# Include directories
include_directories(include)

# Create library
add_library(biomesh_pdb STATIC
    src/PDBReader.cpp
)

target_include_directories(biomesh_pdb PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Example executable
add_executable(pdb_example examples/pdb_example.cpp)
target_link_libraries(pdb_example biomesh_pdb)

# Install targets
install(TARGETS biomesh_pdb pdb_example
    EXPORT BioMeshTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)

# Export targets
install(EXPORT BioMeshTargets
    FILE BioMeshTargets.cmake
    NAMESPACE BioMesh::
    DESTINATION lib/cmake/BioMesh
)

# Create package config
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "BioMeshConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/BioMeshConfigVersion.cmake"
    DESTINATION lib/cmake/BioMesh
)