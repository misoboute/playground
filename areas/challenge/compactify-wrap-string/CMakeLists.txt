fob_find_or_build(GTest REQUIRED)

include_directories(${PLAYGROUND_ROOT}/include/test)

include_directories(${GTEST_INCLUDE_DIRS})
add_definitions(-DGTEST_LANG_CXX11=1)

set(SOURCES 
    main.cpp
)

add_executable(compactify_wrap_string ${SOURCES})

target_link_libraries(compactify_wrap_string GTest::gtest_main)
