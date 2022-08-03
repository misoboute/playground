set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Network WebChannel WebSockets Widgets)

if (NOT Qt5_FOUND)
    return()
endif()

include_directories(${PLAYGROUND_ROOT}/include/test)

add_definitions(-DBUILD_DIR=\"${CMAKE_CURRENT_SOURCE_DIR}/standalone\")
set(SOURCES 
    shared/websocketclientwrapper.cpp
    shared/websocketclientwrapper.h
    shared/websockettransport.cpp
    shared/websockettransport.h
    standalone/core.h
    standalone/dialog.cpp
    standalone/dialog.h
    standalone/dialog.ui
    standalone/main.cpp
)

add_executable(qt5-webchannel-example ${SOURCES})

target_link_libraries(qt5-webchannel-example 
    Qt5::Network Qt5::WebChannel Qt5::WebSockets Qt5::Widgets)

sm_deployqt(qt5-webchannel-example)
