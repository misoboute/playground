find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${PLAYGROUND_ROOT}/include/test)

include_directories(${GTEST_INCLUDE_DIRS})
add_definitions(-DGTEST_LANG_CXX11=1)

set(SOURCES 
    main.cpp
)

add_executable(adjacent_coins ${SOURCES})

target_link_libraries(adjacent_coins
    Threads::Threads
    ${GTEST_BOTH_LIBRARIES}
)
