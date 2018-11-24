set(CMAKE_AUTOMOC ON)

find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${SM_PLANNING_TOOL_DIR}/src/include/test_util)

include_directories(${GTEST_INCLUDE_DIRS})
add_definitions(-DGTEST_LANG_CXX11=1)

set(SOURCES 
    main.cpp
)

add_executable(challenge_tomtom ${SOURCES})

target_link_libraries(challenge_tomtom
    Threads::Threads
    ${GTEST_BOTH_LIBRARIES}
)
