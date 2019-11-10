find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${PLAYGROUND_ROOT}/include/test)

include_directories(${GTEST_INCLUDE_DIRS})
add_definitions(-DGTEST_LANG_CXX11=1)

set(SOURCES 
    main.cpp
)

add_executable(single_swap_sort ${SOURCES})

target_link_libraries(single_swap_sort
    Threads::Threads
    ${GTEST_BOTH_LIBRARIES}
)
