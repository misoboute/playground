CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(json_spirit)

set(CXX_STANDARD 11)
set(CXX_STANDARD_REQUIRED ON)

INCLUDE_DIRECTORIES(json_spirit)

OPTION(JSON_SPIRIT_WVALUE_ENABLED "Build with support for unicode types" OFF)
OPTION(JSON_SPIRIT_MVALUE_ENABLED "Build with support for ascii map types" OFF)
OPTION(JSON_SPIRIT_WMVALUE_ENABLED "Build with support for unicode map types" OFF)

OPTION(JSON_SPIRIT_DEMOS "Enable building demos" ON)
OPTION(JSON_SPIRIT_TESTS "Enable building tests" ON)

set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/PDBFilesDebug)

add_subdirectory(json_spirit)

if(JSON_SPIRIT_DEMOS)
    add_subdirectory(json_demo)
    add_subdirectory(json_headers_only_demo)
    if(JSON_SPIRIT_MVALUE_ENABLED)
        # Can only be used when map values are enabled
        add_subdirectory(json_map_demo)
    endif(JSON_SPIRIT_MVALUE_ENABLED)
endif(JSON_SPIRIT_DEMOS)

if(JSON_SPIRIT_TESTS)
    add_subdirectory(json_test)
endif(JSON_SPIRIT_TESTS)

set(CPACK_PACKAGE_VERSION_MAJOR 4)
set(CPACK_PACKAGE_VERSION_MINOR 0)
set(CPACK_PACKAGE_VERSION_PATCH 8)
set(CPACK_PACKAGE_DESCRIPTION
    "A C++ JSON Parser/Generator Implemented with Boost Spirit")
include(CPack)

install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG}/json_spirit.pdb
    DESTINATION lib OPTIONAL)
