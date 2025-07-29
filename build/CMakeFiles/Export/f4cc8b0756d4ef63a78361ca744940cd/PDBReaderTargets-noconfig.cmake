#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "PDBReader::pdbreader" for configuration ""
set_property(TARGET PDBReader::pdbreader APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(PDBReader::pdbreader PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "${_IMPORT_PREFIX}/lib/libpdbreader.a"
  )

list(APPEND _cmake_import_check_targets PDBReader::pdbreader )
list(APPEND _cmake_import_check_files_for_PDBReader::pdbreader "${_IMPORT_PREFIX}/lib/libpdbreader.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
