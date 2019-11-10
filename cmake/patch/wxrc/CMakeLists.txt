# Added by swissmeda
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wxrc wxrc.cpp)

find_package(wxWidgets COMPONENTS xml base REQUIRED)
include(${wxWidgets_USE_FILE})
target_link_libraries(wxrc ${wxWidgets_LIBRARIES})

install(TARGETS wxrc DESTINATION bin)
