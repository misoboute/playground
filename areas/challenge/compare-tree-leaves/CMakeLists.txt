fob_find_or_build(GTest CONFIG REQUIRED)

add_definitions(-DGTEST_LANG_CXX11=1)

set(SOURCES 
    main.cpp
)

add_executable(compare_tree_leaves ${SOURCES})

target_link_libraries(compare_tree_leaves GTest::gtest_main)
