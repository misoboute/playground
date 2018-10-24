list(APPEND CMAKE_PREFIX_PATH 
    ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/Qt5.11.1/msvc2017_64
)
sm_find_or_get(Qt5
    COMPONENTS WebChannel WebEngineWidgets 
    VERIF_TARGET Qt5::WebEngineWidgets REQUIRED
)

include_directories(${SM_PLANNING_TOOL_DIR}/src/include/test_util)

set(SOURCES 
    main.cpp
)

add_executable(qt-webchannel ${SOURCES})

target_link_libraries(qt-webchannel Qt5::WebEngineWidgets)

sm_deployqt(qt-webchannel)
