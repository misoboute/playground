set(CMAKE_AUTOMOC ON)

list(APPEND CMAKE_PREFIX_PATH ${SM_3RDPARTY_INSTALL_DIR}/Qt5.11.1/msvc2017_64)
sm_find_or_get(Qt5
    COMPONENTS WebChannel WebEngineWidgets 
    VERIF_TARGET Qt5::WebEngineWidgets REQUIRED
)

include_directories(${PLAYGROUND_ROOT}/include/test)

add_definitions(-DINDEX_HTML_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/index.html\")

set(SOURCES 
    Cases.cpp
    SmopIface.cpp
    main.cpp
)

add_executable(qt-webchannel ${SOURCES})

target_link_libraries(qt-webchannel Qt5::WebEngineWidgets)

sm_deployqt(qt-webchannel)
