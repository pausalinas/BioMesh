# Examples CMakeLists.txt

# Basic usage example
add_executable(basic_usage basic_usage.cpp)
target_link_libraries(basic_usage PRIVATE pdbreader)

# Advanced usage example
add_executable(advanced_usage advanced_usage.cpp)
target_link_libraries(advanced_usage PRIVATE pdbreader)

# Set executable properties
set_target_properties(basic_usage advanced_usage PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/examples
)