cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(playground)

get_filename_component(SM_PLANNING_TOOL_DIR 
    ${CMAKE_SOURCE_DIR}/../planning_program_win ABSOLUTE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SM_PLANNING_TOOL_DIR}/src/cmake)

include(SwissMedaUtil)

sm_find_or_get(VTK 8 REQUIRED)

include_directories(${SM_PLANNING_TOOL_DIR}/src/include/test_util)

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})
