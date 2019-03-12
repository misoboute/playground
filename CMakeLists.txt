cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
# Required for nlohmann/json at least:
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(playground)

set(SM_PLANNING_TOOL_DIR "" CACHE PATH
    "Root directory of the swissmeda planning tool project. If set, this will \
add swissmeda cmake modules directory to CMAKE_MODULE_PATH and include \
SwissMedaUtil."
)
if (SM_PLANNING_TOOL_DIR)
    list(APPEND CMAKE_MODULE_PATH ${SM_PLANNING_TOOL_DIR}/src/cmake)
    include(SwissMedaUtil)
endif (SM_PLANNING_TOOL_DIR)

set(SM_ALL_AREAS 
    challenge/adjacent-coins
    challenge/codility-demo
    challenge/cybercom1
    challenge/concept-storyboard
    challenge/cybercom2
    challenge/cybercom3
    challenge/hackerrank/bit-array
    challenge/hackerrank/variadics
    challenge/intersect-circles
    challenge/maven-1
    challenge/maven-2
    challenge/maven-3
    challenge/single-swap-sort
    challenge/tomtom-1
    challenge/tomtom-3
    challenge/tomtom-4
    MoveCopyCtors
    pedastep
    qt-webchannel
    qt5-webchannel-example
)

foreach (_area ${SM_ALL_AREAS})
    set(_area_opt_name SM_BUILD_${_area})
    string(MAKE_C_IDENTIFIER ${_area_opt_name} _area_opt_name)
    string(TOUPPER ${_area_opt_name} _area_opt_name)
    option(${_area_opt_name} 
        "Generate the ${_area} sub-area of the playground" OFF)
    if (${_area_opt_name})
        add_subdirectory(areas/${_area})
    endif (${_area_opt_name})
endforeach (_area)
