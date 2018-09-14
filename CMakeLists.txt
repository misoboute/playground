cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)

project(playground)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${SM_PLANNING_TOOL_DIR}/src/cmake)

include(SwissMedaUtil)

set(CMAKE_PREFIX_PATH 
    ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/smopapp ${CMAKE_PREFIX_PATH})

sm_find_or_get(VTK 8.1 EXTRA_PREFIX smopapp REQUIRED)

include(${VTK_USE_FILE})

include_directories(${SM_PLANNING_TOOL_DIR}/src/include/test_util)
include_directories(${VTK_INCLUDE_DIRS})

set(SOURCES main.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${VTK_LIBRARIES})