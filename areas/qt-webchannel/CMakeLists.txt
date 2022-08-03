set(CMAKE_AUTOMOC ON)

include(FOBBootstrap)

find_package(Qt5 COMPONENTS WebChannel)

if (NOT Qt5_FOUND)
    return()
endif()

include_directories(${PLAYGROUND_ROOT}/include/test)

add_definitions(-DINDEX_HTML_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/index.html\")

set(SOURCES 
    Cases.cpp
    SmopIface.cpp
    main.cpp
)

add_executable(qt-webchannel ${SOURCES})

target_link_libraries(qt-webchannel Qt6::WebChannel)

# sm_deployqt(qt-webchannel)
