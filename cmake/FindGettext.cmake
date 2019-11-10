set(TEMP_CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})
unset(CMAKE_MODULE_PATH)
include(FindGettext)
set(CMAKE_MODULE_PATH ${TEMP_CMAKE_MODULE_PATH})

get_filename_component(_gettext_dir ${GETTEXT_MSGMERGE_EXECUTABLE} DIRECTORY)
find_program(GETTEXT_XGETTEXT_EXECUTABLE xgettext HINTS ${_gettext_dir})
find_program(GETTEXT_MSGINIT_EXECUTABLE msginit HINTS ${_gettext_dir})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Gettext DEFAULT_MSG
    GETTEXT_XGETTEXT_EXECUTABLE GETTEXT_MSGINIT_EXECUTABLE)

# Based on the location of *_INLCUDE_DIRS, sm_find_or_get decides whether it
# has found the right package (built by SMUtil):
get_filename_component(
    Gettext_INCLUDE_DIRS ${GETTEXT_XGETTEXT_EXECUTABLE} DIRECTORY)

mark_as_advanced(
    GETTEXT_XGETTEXT_EXECUTABLE
    GETTEXT_MSGINIT_EXECUTABLE
    Gettext_INCLUDE_DIRS
)

# Adds a custom command to extract translatable strings from a set of input
# source files into an output pot file using xgettext.
# sm_add_xgettext_cmd(output.pot input1.cpp input2.cpp ... OPTIONS -k_ -s ...)
function(sm_add_xgettext_cmd _output_file)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)
    cmake_parse_arguments(AXC
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(_input_files ${AXC_UNPARSED_ARGUMENTS})

    if (NOT _input_files)
        message(FATAL_ERROR "No input source files provided!")
    endif (NOT _input_files)

    get_filename_component(_output_file ${_output_file} ABSOLUTE
        BASEDIR ${CMAKE_CURRENT_BINARY_DIR})
    get_filename_component(_output_dir ${_output_file} DIRECTORY)
    get_filename_component(_output_filename ${_output_file} NAME)
    get_filename_component(_output_filebase ${_output_file} NAME_WE)

    set(_input_list_file
        ${CMAKE_CURRENT_BINARY_DIR}/${_output_filebase}-input.txt)
    file(WRITE ${_input_list_file} "")
    set(_input_files_abs)
    foreach(_input ${_input_files})
        get_filename_component(_input ${_input} ABSOLUTE)
        file(APPEND ${_input_list_file} "${_input}\n")
        list(APPEND _input_files_abs ${_input})
    endforeach(_input)

    add_custom_command(OUTPUT ${_output_file}
        COMMAND ${GETTEXT_XGETTEXT_EXECUTABLE} ${AXC_OPTIONS}
            -p ${_output_dir} -o ${_output_filename} -f ${_input_list_file}
        DEPENDS ${_input_files_abs}
        COMMENT "Extracting translatable strings template: ${_output_file}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM USES_TERMINAL
    )
endfunction(sm_add_xgettext_cmd)

# Adds a custom command to update an existing po file using msgmerge or
# initialize a new one using msginit (if it doesn't exist) from a template
# pot file.
# sm_add_msgmerge_or_msginit_cmd(output.po input.pot
#    INIT_OPTIONS -l en_UK.UTF-8 ...
#    MERGE_OPTIONS --backup=existing -F ...)
function(sm_add_msgmerge_or_msginit_cmd _output_file _input_file)
    set(options)
    set(oneValueArgs)
    set(multiValueArgs INIT_OPTIONS MERGE_OPTIONS)
    cmake_parse_arguments(AMMC
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    get_filename_component(_output_file ${_output_file} ABSOLUTE
        BASEDIR ${CMAKE_CURRENT_BINARY_DIR})

    if (EXISTS ${_output_file})
        set(_command ${GETTEXT_MSGMERGE_EXECUTABLE}
            ${AMMC_MERGE_OPTIONS} -U ${_output_file} ${_input_file})
    else (EXISTS ${_output_file})
        set(_command ${GETTEXT_MSGINIT_EXECUTABLE} --no-translator
            ${AMMC_INIT_OPTIONS} -o ${_output_file} -i ${_input_file})
    endif (EXISTS ${_output_file})
    add_custom_command(OUTPUT ${_output_file}
        COMMAND ${_command}
        MAIN_DEPENDENCY ${_input_file}
        COMMENT "Initializing/updating message translation: ${_output_file}"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        VERBATIM USES_TERMINAL
    )
endfunction(sm_add_msgmerge_or_msginit_cmd)
