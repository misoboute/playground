include_directories(${PLAYGROUND_ROOT}/include/test)

include_directories(${GTEST_INCLUDE_DIRS})

set(SOURCES 
    Interview1.cpp
)

add_executable(concept_storyboard ${SOURCES})
