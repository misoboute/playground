cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(playground)

set(PLAYGROUND_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

list(APPEND CMAKE_MODULE_PATH ${PLAYGROUND_ROOT}/cmake)
include(FOBBootstrap)

set(SM_ALL_AREAS 
    BenchmarkConsecutiveInserts
    CodersStrikeBack
    MarjanQML
    MoveCopyCtors
    challenge/SplitArrayLargestSum
    challenge/adjacent-coins
    challenge/amazon-1
    challenge/amazon-2
    challenge/arithmetic-slices
    challenge/cafe-bazaar
    challenge/cafe-bazaar-2
    challenge/codility-demo
    challenge/concept-storyboard
    challenge/cybercom1
    challenge/cybercom2
    challenge/cybercom3
    challenge/gameloft-2
    challenge/hackerrank/bit-array
    challenge/hackerrank/variadics
    challenge/intersect-circles
    challenge/largest-plus-sign
    challenge/maven-1
    challenge/maven-2
    challenge/maven-3
    challenge/optiver
    challenge/sequential-digits
    challenge/single-swap-sort
    challenge/tomtom-1
    challenge/tomtom-3
    pedastep
    qt-webchannel
    qt5-webchannel-example
)

foreach (_area ${SM_ALL_AREAS})
    set(_area_opt_name SM_BUILD_${_area})
    string(MAKE_C_IDENTIFIER ${_area_opt_name} _area_opt_name)
    string(TOUPPER ${_area_opt_name} _area_opt_name)
    option(${_area_opt_name} 
        "Generate the ${_area} sub-area of the playground" ON)
    if (${_area_opt_name})
        add_subdirectory(areas/${_area})
    endif (${_area_opt_name})
endforeach (_area)
