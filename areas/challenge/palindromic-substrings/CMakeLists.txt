fob_find_or_build(GTest CONFIG REQUIRED)

add_definitions(-DGTEST_LANG_CXX11=1)

set(SOURCES 
    main.cpp
)

add_executable(palindromic_substrings ${SOURCES})

target_link_libraries(palindromic_substrings GTest::gtest_main)
