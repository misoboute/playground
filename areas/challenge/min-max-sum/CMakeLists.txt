fob_find_or_build(GTest CONFIG REQUIRED)

add_definitions(-DGTEST_LANG_CXX11=1)

set(SOURCES 
    main.cpp
)

add_executable(min_max_sum ${SOURCES})

target_link_libraries(min_max_sum GTest::gtest_main)
