include_directories(${SM_PLANNING_TOOL_DIR}/src/include/test_util)

include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCES 
    Interview1.cpp
)

add_executable(concept_storyboard ${SOURCES})
