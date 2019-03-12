include_directories(${SM_PLANNING_TOOL_DIR}/src/include/test_util)

set(SOURCES 
    main.cpp
)

add_executable(move-copy-ctors ${SOURCES})
