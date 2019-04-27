set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES 
    main.cpp
    qml.qrc
)

include(SwissMedaUtil)

find_package(Qt5 COMPONENTS Widgets Qml Quick REQUIRED)

add_executable(MarjanQML MACOSX_BUNDLE ${SOURCES})

target_link_libraries(MarjanQML Qt5::Widgets Qt5::Qml Qt5::Quick)

sm_deployqt(MarjanQML)
