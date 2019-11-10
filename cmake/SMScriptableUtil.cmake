
# Use this function to create a symbolic link during the configuration in a
# platform independent manner. If the symlink already exists, it does not
# change it. On Windows, configuration step must be done with administrative
# privileges for symbolic link creation to be successful.
function(sm_create_symlink LINK_NAME LINK_TARGET)
    if(EXISTS ${LINK_NAME})
        return()
    endif(EXISTS ${LINK_NAME})
    get_filename_component(_dest_dir ${LINK_NAME} DIRECTORY)
    if (NOT EXISTS _dest_dir)
        file(MAKE_DIRECTORY ${_dest_dir})
    endif (NOT EXISTS _dest_dir)
    file(TO_NATIVE_PATH ${LINK_NAME} LINK_NAME)
    file(TO_NATIVE_PATH ${LINK_TARGET} LINK_TARGET)
    if(WIN32)
        set(CREATE_SYMLINK_COMMAND
            $ENV{ComSpec} /C mklink /D ${LINK_NAME} ${LINK_TARGET})
    else(WIN32)
        set(CREATE_SYMLINK_COMMAND
            ${CMAKE_COMMAND} -E create_symlink ${LINK_TARGET} ${LINK_NAME})
    endif(WIN32)
    message("Creating symbolic link from ${LINK_NAME} to ${LINK_TARGET}")
    execute_process(
        COMMAND ${CREATE_SYMLINK_COMMAND}
        OUTPUT_VARIABLE CMD_OUTPUT
        ERROR_VARIABLE CMD_OUTPUT
    )
    if(CMD_OUTPUT)
        message(${CMD_OUTPUT})
    endif(CMD_OUTPUT)
endfunction(sm_create_symlink LINK_NAME LINK_TARGET)

# Use this function to delete symbolic links. If one of the links does not
# exists, it simply ignores it and moves on. All the symlinks as a list.
function(sm_remove_symlink)
    foreach(LINK ${ARGN})
        if(EXISTS ${LINK})
            get_filename_component(LINK ${LINK} ABSOLUTE)
            file(TO_NATIVE_PATH ${LINK} LINK_NATIVE)
            message("Removing symbolic link: ${LINK_NATIVE}")
            if(WIN32)
                if(IS_SYMLINK ${LINK})
                    execute_process(
                        COMMAND fsutil reparsepoint delete ${LINK_NATIVE})
                endif(IS_SYMLINK ${LINK})
                file(REMOVE_RECURSE ${LINK})
            else(WIN32)
                if(IS_SYMLINK ${LINK})
                    execute_process(COMMAND unlink ${LINK_NATIVE})
                endif(IS_SYMLINK ${LINK})
            endif(WIN32)
        endif(EXISTS ${LINK})
    endforeach(LINK ${LINKS})
endfunction(sm_remove_symlink LINK_NAME LINK_TARGET)

# Export the file system based variables (<prefix>/var/...) and their values
# as environment variables to a shell script file that initializes the
# environment. The default prefix is empty (it will dumpt system wide vars on
# UNIX). For example, if directory <prefix>/var contains the following files
# with the following contents,
#
# File                      Contents
# ----------------------    ---------------------
# <prefix>/var/version      2.14.0.1699
# <prefix>/var/outputurl    http://www.example.com/buildoutput
# <prefix>/var/cat/var1     Some value
# <prefix>/var/cat/var.out  Some other value
#
# Then the output file will contain
#
# version=2.14.0.1699
# outputurl=http://www.example.com/buildoutput
# cat_var1=Some value
# cat_var_out=Some other value
#
# If the output file already exists, its contents will be overwritten.
# All non-portable characters in variable names (anything except alphanumeric
# and underscore) will be converted to underscore in the exported file.
function(sm_export_filesys_vars_to_envvar_file OUTFILE)
    set(options )
    set(oneValueArgs DIR_PREFIX VAR_PREFIX)
    set(multiValueArgs )
    cmake_parse_arguments(EXP
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    file(WRITE ${OUTFILE})
    file(GLOB_RECURSE _var_files LIST_DIRECTORIES false
        RELATIVE ${EXP_DIR_PREFIX}/var ${EXP_DIR_PREFIX}/var/*)
    foreach(_var_file ${_var_files})
        string(MAKE_C_IDENTIFIER "${EXP_VAR_PREFIX}${_var_file}" _var_name)
        file(READ ${EXP_DIR_PREFIX}/var/${_var_file} _var_value)
        string(REGEX MATCH "[\n\"\;,`'\\|]" _special_chars "${_var_value}" )
        if(_special_chars)
            string(REPLACE "'" "\\'" _var_value "${_var_value}")
            set(_var_value "'${_var_value}'")
        endif(_special_chars)
        file(APPEND ${OUTFILE} "${_var_name}=${_var_value}\n")
    endforeach(_var_file)
endfunction(sm_export_filesys_vars_to_envvar_file)

# Concatenate a number of files into a single output file. The first argument
# is the output file and the reset of the arguments are the input files. The
# input files will be concatenated in the same order as listed.
function (sm_concat_files OUTFILE)
    file(REMOVE ${OUTFILE})
    file(TO_NATIVE_PATH "${OUTFILE}" OUTFILE)
    set(_input_files)
    foreach (_in_file ${ARGN})
        file(TO_NATIVE_PATH "${_in_file}" _in_file_native)
        list(APPEND _input_files ${_in_file_native})
    endforeach (_in_file)
    if (WIN32)
        string(REPLACE ";" ";+;" _input_files "${_input_files}")
        set(_concat_command
            $ENV{ComSpec} /C copy /Y /B ${_input_files} ${OUTFILE})
    elseif (UNIX)
        message(AUTHOR_WARNING "concat_files not implemented for Unix")
    endif (WIN32)
    execute_process(
        COMMAND ${_concat_command}
        OUTPUT_VARIABLE CMD_OUTPUT
        ERROR_VARIABLE CMD_OUTPUT
    )
    if(CMD_OUTPUT)
        message(${CMD_OUTPUT})
    endif(CMD_OUTPUT)
endfunction (sm_concat_files)

# Set a file-system-based variable. The variable is a file and the value is the
# contents of the file. This is like the /var directory on UNIX. Set The
# cache variable SM_FILESYSTEM_VARIABLE_DIR to a new prefix (something like
# ${CMAKE_CURRENT_BINARY_DIR}/var) if you don't want the variables to be
# stored in the global unix /var (and destroyed on reboot).
function(sm_set_filesys_variable VARNAME VARVALUE)
    file(WRITE ${SM_FILESYSTEM_VARIABLE_DIR}/${VARNAME} ${VARVALUE})
endfunction(sm_set_filesys_variable)

# Ge the value of file-system-based variable. The variable must have been
# previously set by a call to sm_set_filesys_variable. If the value of the
# variable is not set, the value will be empty. See sm_set_filesys_variable
# for more info.
function(sm_get_filesys_variable VARNAME OUTPUTVAR)
    set(_var_file ${SM_FILESYSTEM_VARIABLE_DIR}/${VARNAME})
    if (EXISTS ${_var_file})
        file(READ ${SM_FILESYSTEM_VARIABLE_DIR}/${VARNAME} _value)
        set(${OUTPUTVAR} ${_value} PARENT_SCOPE)
    else (EXISTS ${_var_file})
        unset(${OUTPUTVAR} PARENT_SCOPE)
    endif (EXISTS ${_var_file})
endfunction(sm_get_filesys_variable)

# Make sure that a certain file system based variable exists
function(sm_set_default_filesys_var_value VARNAME DEFAULT_VALUE)
    if (NOT EXISTS ${SM_FILESYSTEM_VARIABLE_DIR}/${VARNAME})
        sm_set_filesys_variable(${VARNAME} ${DEFAULT_VALUE})
    endif (NOT EXISTS ${SM_FILESYSTEM_VARIABLE_DIR}/${VARNAME})
endfunction(sm_set_default_filesys_var_value)

# Split a string into a ;-list using the given separator string.
function(sm_split_string STR SEPARATOR OUTPUTVAR)
    string(LENGTH ${SEPARATOR} SEP_LEN)
    set(POSITION 0)
    set(OUTPUT_LIST)
    while (NOT POSITION EQUAL -1)
        string(FIND "${STR}" "${SEPARATOR}" POSITION)
        string(SUBSTRING "${STR}" 0 ${POSITION} LIST_ITEM)
        if (POSITION GREATER -1)
            math(EXPR NEXT_START_POS "${POSITION} + ${SEP_LEN}")
            string(SUBSTRING "${STR}" ${NEXT_START_POS} -1 STR)
        endif ()
        list(APPEND OUTPUT_LIST "${LIST_ITEM}")
    endwhile ()
    set(${OUTPUTVAR} ${OUTPUT_LIST} PARENT_SCOPE)
endfunction(sm_split_string)

# Extract a version number (2 to 4 components) from a string and place its
# different components in multiple variables which share a specified prefix.
# The version variables are the same as those set by CMake find modules.
function(sm_extract_version STR PREFIX)
    set(${PREFIX}_VERSION "" PARENT_SCOPE)
    set(${PREFIX}_VERSION_COUNT 0 PARENT_SCOPE)
    set(${PREFIX}_VERSION_MAJOR 0 PARENT_SCOPE)
    set(${PREFIX}_VERSION_MINOR 0 PARENT_SCOPE)
    set(${PREFIX}_VERSION_PATCH 0 PARENT_SCOPE)
    set(${PREFIX}_VERSION_TWEAK 0 PARENT_SCOPE)

    string(REGEX MATCH "[0-9]+\\.[0-9]+(\\.[0-9]+)?(\\.[0-9]+)?" VERSION ${STR})
    if (NOT VERSION)
        return()
    endif (NOT VERSION)

    sm_split_string(${VERSION} "." COMPONENTS)
    list(LENGTH COMPONENTS COUNT)

    list(GET COMPONENTS 0 MAJOR)
    list(GET COMPONENTS 1 MINOR)

    set(${PREFIX}_VERSION ${VERSION} PARENT_SCOPE)
    set(${PREFIX}_VERSION_COUNT ${COUNT} PARENT_SCOPE)
    set(${PREFIX}_VERSION_MAJOR ${MAJOR} PARENT_SCOPE)
    set(${PREFIX}_VERSION_MINOR ${MINOR} PARENT_SCOPE)

    if (COUNT GREATER_EQUAL 3)
        list(GET COMPONENTS 2 PATCH)    
        set(${PREFIX}_VERSION_PATCH ${PATCH} PARENT_SCOPE)
    endif ()

    if (COUNT EQUAL 4)
        list(GET COMPONENTS 3 TWEAK)
        set(${PREFIX}_VERSION_TWEAK ${TWEAK} PARENT_SCOPE)
    endif ()
endfunction()

# Iterate a directory recursively and convert the list of files to a QRC file
# which can be used to embed the files in the Qt resources system of an
# executable.
function(sm_qrc_from_dir IN_DIR_PATH OUT_QRC_PATH PREFIX)
    file(WRITE ${OUT_QRC_PATH} "<RCC>\n    <qresource prefix=\"${PREFIX}\">\n")
    file(GLOB_RECURSE FILE_LIST
        LIST_DIRECTORIES false ${IN_DIR_PATH}/**)
    get_filename_component(OUT_QRC_DIR ${OUT_QRC_PATH} DIRECTORY)
    foreach(RES_FILE ${FILE_LIST})
        if (OUT_QRC_PATH STREQUAL RES_FILE)
            continue()
        endif ()
        file(RELATIVE_PATH REL_RES_PATH ${OUT_QRC_DIR} ${RES_FILE})
        file(APPEND ${OUT_QRC_PATH} "        <file>${REL_RES_PATH}</file>\n")
    endforeach(RES_FILE)
    file(APPEND ${OUT_QRC_PATH} "    </qresource>\n</RCC>\n")
endfunction(sm_qrc_from_dir)
