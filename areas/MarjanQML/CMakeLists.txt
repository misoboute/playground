set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(SOURCES 
    main.cpp
    qml.qrc
)

fob_find_or_build(Qt6 COMPONENTS Widgets Qml Quick REQUIRED)

add_executable(MarjanQML MACOSX_BUNDLE ${SOURCES})

target_link_libraries(MarjanQML Qt6::Widgets Qt6::Qml Qt6::Quick)

# sm_deployqt(MarjanQML)
