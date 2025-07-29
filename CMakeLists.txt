cmake_minimum_required(VERSION 3.16)

project(PDBReader 
    VERSION 1.0.0
    DESCRIPTION "C++ library for reading and parsing PDB structure files"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler flags for better error reporting and optimization
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra -Wpedantic -Werror)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-g -O0 -fsanitize=address)
        add_link_options(-fsanitize=address)
    else()
        add_compile_options(-O3 -DNDEBUG)
    endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/W4 /WX)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(/Zi /Od)
    else()
        add_compile_options(/O2 /DNDEBUG)
    endif()
endif()

# Include directories
include_directories(include)

# Library source files
set(LIBRARY_SOURCES
    src/Atom.cpp
    src/PDBReader.cpp
)

set(LIBRARY_HEADERS
    include/Atom.h
    include/PDBReader.h
)

# Create the main library
add_library(pdbreader STATIC ${LIBRARY_SOURCES} ${LIBRARY_HEADERS})

# Set include directories for the library
target_include_directories(pdbreader PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Set library properties
set_target_properties(pdbreader PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
    PUBLIC_HEADER "${LIBRARY_HEADERS}"
)

# Option to build tests
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    # Find Google Test
    find_package(GTest QUIET)
    
    if(NOT GTest_FOUND)
        # Download and build Google Test if not found
        include(FetchContent)
        
        FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1
        )
        
        # For Windows: Prevent overriding the parent project's compiler/linker settings
        set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
        
        FetchContent_MakeAvailable(googletest)
    endif()

    # Enable testing
    enable_testing()

    # Test source files
    set(TEST_SOURCES
        tests/test_atom.cpp
        tests/test_pdbreader.cpp
    )

    # Create test executable
    add_executable(pdbreader_tests ${TEST_SOURCES})

    # Link test executable with library and Google Test
    target_link_libraries(pdbreader_tests 
        PRIVATE 
        pdbreader
        GTest::gtest_main
        GTest::gtest
    )

    # Add tests to CTest
    include(GoogleTest)
    gtest_discover_tests(pdbreader_tests)

    # Custom target to run tests
    add_custom_target(run_tests
        COMMAND pdbreader_tests
        DEPENDS pdbreader_tests
        COMMENT "Running unit tests"
    )
endif()

# Option to build examples
option(BUILD_EXAMPLES "Build example programs" ON)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Install rules
install(TARGETS pdbreader
    EXPORT PDBReaderTargets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    PUBLIC_HEADER DESTINATION include
)

# Install CMake config files
install(EXPORT PDBReaderTargets
    FILE PDBReaderTargets.cmake
    NAMESPACE PDBReader::
    DESTINATION lib/cmake/PDBReader
)

# Create config file
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    PDBReaderConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/PDBReaderConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PDBReaderConfig.cmake
    INSTALL_DESTINATION lib/cmake/PDBReader
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/PDBReaderConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/PDBReaderConfigVersion.cmake
    DESTINATION lib/cmake/PDBReader
)

# Export targets for build tree
export(EXPORT PDBReaderTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/PDBReaderTargets.cmake
    NAMESPACE PDBReader::
)

# Print build configuration
message(STATUS "PDBReader Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ compiler: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "  C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "  Build examples: ${BUILD_EXAMPLES}")