include(${CMAKE_CURRENT_LIST_DIR}/SMScriptableUtil.cmake)

message("Processing pull request long description:\n")
set(PR_DESC "$ENV{ghprbPullLongDescription}")
if (WIN32)
    # Maybe running under a shell that transforms mixed back/forward slashes
    # in environment variables to all forward slashes.
    string(REPLACE "/r/n" "\\r\\n" PR_DESC "${PR_DESC}")
endif ()
sm_split_string("${PR_DESC}" "\\r\\n" DESC_LINES)

foreach(DESC_LINE ${DESC_LINES})
    message(${DESC_LINE})
endforeach()

foreach(DESC_LINE ${DESC_LINES})
    if (DESC_LINE MATCHES "^BUILD_VAR_([A-Za-z][A-Za-z0-9_]*)=(.*)\$")
        set(VAR_NAME ${CMAKE_MATCH_1})
        set(VAR_VALUE ${CMAKE_MATCH_2})
        sm_set_filesys_variable(${VAR_NAME} ${VAR_VALUE})
        message(STATUS "Build var ${VAR_NAME} set to ${VAR_VALUE}")
    endif ()
endforeach()
    
sm_set_default_filesys_var_value(ENABLE_LEGACY_PT_AND_SHELL_WIN64 0)
sm_set_default_filesys_var_value(ENABLE_LEGACY_PT_AND_SHELL_MACOS 0)
sm_set_default_filesys_var_value(ENABLE_UNIFIED_APP_WIN64 1)
sm_set_default_filesys_var_value(ENABLE_UNIFIED_APP_MACOS 1)
sm_set_default_filesys_var_value(PRODUCT_NAME smop)
