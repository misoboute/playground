fob_find_or_build(GTest CONFIG REQUIRED)

add_definitions(-DGTEST_LANG_CXX11=1)

set(SOURCES 
    main.cpp
)

add_executable(balanced_braces ${SOURCES})

target_link_libraries(balanced_braces GTest::gtest_main)
