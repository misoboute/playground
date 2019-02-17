list(APPEND CMAKE_PREFIX_PATH 
    ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/Qt5.11.1/msvc2017_64
)

find_package(Qt5 COMPONENTS Core)

include_directories(${SM_PLANNING_TOOL_DIR}/src/include/test_util)

set(SOURCES 
    main.cpp
)

add_executable(pedastep ${SOURCES})

target_link_libraries(pedastep Qt5::Core)

sm_deployqt(pedastep)
