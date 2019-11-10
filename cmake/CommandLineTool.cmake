# Do not include this module directly. It is meant to be used in the list file
# to extend the CMake command line tool mode. Use this module in COMMAND clauses
# as below:
#   ${CMAKE_COMMAND} -P CommandLineTool.cmake
#       -D EXT_COMMAND=<command> -DCMDARGS="ARG1;ARG2;..."
# where <command> is one of the commands defined in this module.

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

include(SMScriptableUtil)

if(${EXT_COMMAND} STREQUAL create_symlink)
    sm_create_symlink(${CMDARGS})
elseif(${EXT_COMMAND} STREQUAL remove_symlink)
    sm_remove_symlink(${CMDARGS})
elseif(${EXT_COMMAND} STREQUAL fatal_error)
    message(FATAL_ERROR "Fatal error: ${CMDARGS}")
elseif(${EXT_COMMAND} STREQUAL export_filesys_vars_to_envvar_file)
    sm_export_filesys_vars_to_envvar_file(${CMDARGS})
elseif(${EXT_COMMAND} STREQUAL concat_files)
    sm_concat_files(${CMDARGS})
elseif(${EXT_COMMAND} STREQUAL qrc_from_dir)
    sm_qrc_from_dir(${CMDARGS})
else(${EXT_COMMAND} STREQUAL create_symlink)
    message(FATAL_ERROR "Unrecognized extended command: ${EXT_COMMAND}")
endif(${EXT_COMMAND} STREQUAL create_symlink)
