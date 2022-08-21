fob_find_or_build(GTest CONFIG REQUIRED)

add_definitions(-DGTEST_LANG_CXX11=1)

set(SOURCES 
    main.cpp
)

add_executable(test_transactions ${SOURCES})

target_link_libraries(test_transactions GTest::gtest_main)
