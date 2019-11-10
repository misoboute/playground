find_package(GTest REQUIRED)
find_package(Threads REQUIRED)

include_directories(${PLAYGROUND_ROOT}/include/test)

include_directories(${GTEST_INCLUDE_DIRS})
add_definitions(-DGTEST_LANG_CXX11=1)

set(SOURCES 
    main.cpp
)

add_executable(hr-bitarray ${SOURCES})

target_link_libraries(hr-bitarray
    Threads::Threads
    ${GTEST_BOTH_LIBRARIES}
)
