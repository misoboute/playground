cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(playground)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SM_PLANNING_TOOL_DIR}/src/cmake)

include(SwissMedaUtil)

sm_find_or_get(HDF5 
    PATH_SUFFIXES hdf5 
    INLCUDE_DIR_VERIF_TARGET hdf5::hdf5-static
)

# sm_find_or_get(HDF5 NO_MODULE)

include_directories(${SM_PLANNING_TOOL_DIR}/src/include/test_util)

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} WIN32 MACOSX_BUNDLE ${SOURCES})
