list(APPEND CMAKE_PREFIX_PATH 
    ${SM_3RDPARTY_INSTALL_DIR}/Qt5.11.1/msvc2017_64
)

find_package(Qt5 COMPONENTS Core)

include_directories(${PLAYGROUND_ROOT}/include/test)

set(SOURCES 
    main.cpp
)

add_executable(pedastep ${SOURCES})

target_link_libraries(pedastep Qt5::Core)

sm_deployqt(pedastep)
