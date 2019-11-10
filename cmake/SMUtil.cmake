cmake_minimum_required(VERSION 3.9)

include(SMScriptableUtil)

# Define SM_IS_64_BIT to identify platform bitness
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SM_IS_64_BIT TRUE)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(SM_IS_64_BIT FALSE)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

# SMUtil specific directories used in build:
# PROGRAMDATA_ROOT is used as a base for download cache, build, and
# installations.
if (WIN32)
    set(_sm_progfiles86_env_var "ProgramFiles(x86)")
    file(TO_CMAKE_PATH "$ENV{${_sm_progfiles86_env_var}}" SM_PROGRAMFILESX86)
    file(TO_CMAKE_PATH "$ENV{ProgramFiles}" SM_PROGRAMFILES)
    file(TO_CMAKE_PATH "$ENV{ProgramData}" PROGRAMDATA_ROOT)

    if(SM_IS_64_BIT)
        set(PROGRAMDATA_ROOT_SM_POSTFIX 64)
        set(SM_ARCHITECTURE x64)
    else(SM_IS_64_BIT)
        set(PROGRAMDATA_ROOT_SM_POSTFIX "")
        set(SM_ARCHITECTURE x86)
    endif(SM_IS_64_BIT)
elseif (APPLE)
    set(PROGRAMDATA_ROOT /usr/local)
endif(WIN32)

set(SM_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR})

set(SM_DIR_NAME misoboute CACHE STRING
    "Base name of the directory. This directory will be created under
the ProgramData directory and everything is placed there.")

set(SM_PROGRAMDATA_DIR
    ${PROGRAMDATA_ROOT}/${SM_DIR_NAME}${PROGRAMDATA_ROOT_SM_POSTFIX}
    CACHE PATH
    "Path to the base directory within ProgramData where everything (download,
source, build, install) is stored.")

set(SM_DOWNLOAD_CACHE_DIR
    ${SM_PROGRAMDATA_DIR}/cache
    CACHE PATH
    "Directory path to be used as cache for retrieving third-party \
libraries if they are not found by find_package procedures.")

set(SM_3RDPARTY_INSTALL_DIR
    ${SM_PROGRAMDATA_DIR}
    CACHE PATH
    "Install prefix for 3rd party libraries retrieved and used by SMUtil. \
The find_* procedures will search this path first.")

# The build directory for the 3rdparty libraries is a shortened hash of the
# combination of various build system configuration parameters. This is to keep
# separate build directories for different build systems. The info about the
# build system used for the build directory could be placed inside the build
# directory using the macro create_3rdparty_build_info_file.
string(MD5 BUILD_SYSTEM_HASH
    "${CMAKE_GENERATOR}${CMAKE_GENERATOR_PLATFORM}${CMAKE_GENERATOR_TOOLSET}")
string(SUBSTRING ${BUILD_SYSTEM_HASH} 0 8 BUILD_SYSTEM_HASH)
set(SM_3RDPARTY_BUILD_DIR
    ${SM_PROGRAMDATA_DIR}/build/${BUILD_SYSTEM_HASH} CACHE PATH
    "Directory path used for the build processes of the third-party libraries.")

set(SM_BUILD_INFO_FILE ${SM_3RDPARTY_BUILD_DIR}/BuildInfo.txt)

set(SM_3RDPARTY_CONFIGURATION_TYPES Debug Release CACHE PATH
    "Configuration types in which 3rd party libraries are built.")

# All the libraries are installed to ${SM_3RDPARTY_INSTALL_DIR}. So
# adding it to the prefix path will ensure that calls to find_package will
# find them there.
if (NOT ${SM_3RDPARTY_INSTALL_DIR} IN_LIST CMAKE_PREFIX_PATH)
    list(APPEND CMAKE_PREFIX_PATH ${SM_3RDPARTY_INSTALL_DIR})
    list(APPEND CMAKE_PROGRAM_PATH ${SM_3RDPARTY_INSTALL_DIR})
endif(NOT ${SM_3RDPARTY_INSTALL_DIR} IN_LIST CMAKE_PREFIX_PATH)

# Bring in ExternalProject so we can easily download and install external
# dependencies. The EP_BASE directory property determines where our build system
# and build artifacts for external projects will go.
include(ExternalProject)
set_directory_properties(PROPERTIES EP_BASE ${SM_3RDPARTY_BUILD_DIR})

# Target upload-aws causes all the files marked for upload (by calling
# sm_upload_to_aws) to be uploaded.
if(NOT TARGET upload-aws)
    add_custom_target(upload-aws
        COMMAND ${CMAKE_COMMAND}
            -E echo "Successfully uploaded all artifacts to AWS."
    )
 endif(NOT TARGET upload-aws)

find_program(CTEST_COMMAND ctest)
find_program(CPACK_COMMAND cpack)
# Target deploy-all can be used to run all tests (CTest), create the installer
# (CPack), and deploy the created package to AWS. All tests added using
# sm_add_tests and all targets installed using sm_install are added to
# the dependencies of deploy-all.
if(NOT TARGET deploy-all)
    add_custom_target(deploy-all
        COMMAND ${CTEST_COMMAND} -C $<CONFIG> -V -j
        COMMAND ${CPACK_COMMAND} -C $<CONFIG> -V
        COMMAND ${CMAKE_COMMAND}
            --build ${CMAKE_BINARY_DIR} --target upload-aws --config $<CONFIG>
        COMMAND ${CMAKE_COMMAND} -E echo "Deployment completed successfully."
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT
            "Running tests and deploying smopapp package to Amazon AWS S3..."
        VERBATIM USES_TERMINAL
    )
endif(NOT TARGET deploy-all)
# This function will be used by subdirectory projects to conveniently create
# a list of header files by only providing the source file names, a prefix
# path where the header files corresponding to the provided source files reside
# (without ending slash), a suffix indicating header file extension (without
# the dot) and an output variable where the generated list is stored.
function(sm_generate_headers_list SOURCE_FILES_VAR HEADER_PREFIX HEADER_SUFFIX
    HEADER_FILES_OUTPUT_VAR)
    if (NOT HEADER_PREFIX)
        set(HEADER_PREFIX .)
    endif (NOT HEADER_PREFIX)
    set(${GENERATED_HEADER_NAMES} "")
    foreach(SOURCE_FILE ${${SOURCE_FILES_VAR}})
        string(REGEX REPLACE "\\/?(.*)\\.(CPP|cpp|CXX|cxx|C{1,2}|c{1,2})"
            "${HEADER_PREFIX}/\\1.${HEADER_SUFFIX}" HEADER_FILE ${SOURCE_FILE})
        list(APPEND GENERATED_HEADER_NAMES ${HEADER_FILE})
    endforeach(SOURCE_FILE)
    set(${HEADER_FILES_OUTPUT_VAR} ${GENERATED_HEADER_NAMES} PARENT_SCOPE)
endfunction(sm_generate_headers_list)

# Use this function to add a prefix to all items of a list. Could be used for
# a list of files all residing in a certain directory.
function(sm_prefix_list LIST_VAR OUT_VAR PREFIX)
    set(PREFIXED_LIST "")
    foreach(ITEM ${${LIST_VAR}})
        list(APPEND PREFIXED_LIST "${PREFIX}${ITEM}")
    endforeach(ITEM)
    set(${OUT_VAR} ${PREFIXED_LIST} PARENT_SCOPE)
endfunction(sm_prefix_list)

# Use this function to create a precompiled header from the given header file
# and use it for all the source files within SOURCES_VAR. It creates a source
# file for the given header in the current binary directory and includes the
# header there - so no need for having a source file for the precompiled header.
# The created source file has the same name as header with a cpp extension.
# The PCH binary is created in the current binary directory and has the same
# name as the precompiled header with a pch extension.
# NOTE: The PCH header must be explicitly included in all the source files
# using it. We are not using forced includes (/FI) because they produce
# "invisible" dependencies and reduces code readability, etc.
function(sm_add_precompiled_header TARGET_OR_SOURCES_VAR PRECOMPILED_HEADER)
    set(options NO_CREATE)
    set(oneValueArgs SOURCE BINARY)
    set(multiValueArgs)
    cmake_parse_arguments(PCH
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    get_filename_component(BASE_NAME ${PRECOMPILED_HEADER} NAME_WE)
    get_filename_component(HEADER_FULL_PATH ${PRECOMPILED_HEADER} ABSOLUTE)
    sm_set_default_var_value(
        PCH_SOURCE ${CMAKE_CURRENT_BINARY_DIR}/${BASE_NAME}.cpp)
    sm_set_default_var_value(
        PCH_BINARY ${CMAKE_CURRENT_BINARY_DIR}/${BASE_NAME}.pch)
    set(_allowed_extensions .c .c++ .cc .cpp .cxx .m .mm)
    if(MSVC)
        if(TARGET ${TARGET_OR_SOURCES_VAR})
            get_target_property(_sources ${TARGET_OR_SOURCES_VAR} SOURCES)
            set_property(TARGET ${TARGET_OR_SOURCES_VAR}
                APPEND PROPERTY SOURCES ${PCH_SOURCE})
        else(TARGET ${TARGET_OR_SOURCES_VAR})
            set(_sources ${${TARGET_OR_SOURCES_VAR}})
            set(${TARGET_OR_SOURCES_VAR}
                ${${TARGET_OR_SOURCES_VAR}} ${PCH_SOURCE} PARENT_SCOPE)
        endif(TARGET ${TARGET_OR_SOURCES_VAR})
        foreach(SOURCE_FILE ${_sources})
            get_filename_component(_src_ext ${SOURCE_FILE} EXT)
            string(TOLOWER ${_src_ext} _src_ext)
            if(NOT _src_ext IN_LIST _allowed_extensions)
                continue()
            endif(NOT _src_ext IN_LIST _allowed_extensions)
            set_property(SOURCE ${SOURCE_FILE}
                APPEND_STRING PROPERTY COMPILE_FLAGS
                    " /Yu\"${PRECOMPILED_HEADER}\" /Fp\"${PCH_BINARY}\""
            )
            set_property(SOURCE ${SOURCE_FILE}
                APPEND PROPERTY OBJECT_DEPENDS ${PCH_BINARY})
        endforeach(SOURCE_FILE)
        if(NOT PCH_NO_CREATE)
            if(NOT EXISTS ${PCH_SOURCE})
                file(WRITE ${PCH_SOURCE} "#include \"${HEADER_FULL_PATH}\"\n")
            endif(NOT EXISTS ${PCH_SOURCE})
            set_source_files_properties(${PCH_SOURCE}
                PROPERTIES
                    COMPILE_FLAGS
                        " /Yc\"${HEADER_FULL_PATH}\" /Fp\"${PCH_BINARY}\""
                    OBJECT_OUTPUTS ${PCH_BINARY}
            )
        endif(NOT PCH_NO_CREATE)
    elseif(XCODE)
        if(TARGET ${TARGET_OR_SOURCES_VAR})
            set_target_properties(${TARGET_OR_SOURCES_VAR}
                PROPERTIES
                    XCODE_ATTRIBUTE_GCC_PREFIX_HEADER ${HEADER_FULL_PATH}
                    XCODE_ATTRIBUTE_GCC_PRECOMPILE_PREFIX_HEADER YES
            )
        else(TARGET ${TARGET_OR_SOURCES_VAR})
            message(AUTHOR_WARNING
                "${TARGET_OR_SOURCES_VAR} is not a target!
Prefix header on macOS can only be enabled for a target!")
        endif(TARGET ${TARGET_OR_SOURCES_VAR})
    endif(MSVC)
endfunction(sm_add_precompiled_header)

# Finds Qt utility windeployqt.exe on Windows and the macdeployqt on macOS
# and executes it on the binary for the given target as a post-build event.
# This will copy allow the executable target to be executed on its own.
# Pass the INSTALL argument to have windeployqt/macdeployqt also run as part
# of the install logic. The passed INSTALL argument specifies the Directory
# relative to the install prefix where windeployqt/macdeployqt should be
# invoked on. The COMPONENT option specifies which install COMPONENT
# will trigger the deployqt.
function(sm_deployqt TARGET_NAME)
    set(options)
    set(oneValueArgs COMPONENT INSTALL)
    set(multiValueArgs)
    cmake_parse_arguments(DQT
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_extra_flags ${DQT_UNPARSED_ARGUMENTS})

    get_target_property(_is_bundle ${TARGET_NAME} MACOSX_BUNDLE)
    if(_is_bundle AND APPLE)
        set(_post_build_loc $<TARGET_BUNDLE_DIR:${TARGET_NAME}>)
    else(_is_bundle AND APPLE)
        set(_post_build_loc $<TARGET_FILE:${TARGET_NAME}>)
    endif(_is_bundle AND APPLE)

    if(APPLE)
        set(_deployqt_exec_name macdeployqt)
        set(_cfg_flag
            $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:-no-strip>)
        set(_deployqt_args ${_post_build_loc} ${_cfg_flag} ${_extra_flags})
    elseif(WIN32)
        set(_deployqt_exec_name windeployqt)
        set(_cfg_flag "$<$<CONFIG:Release>:--release>\
$<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:--debug>")
        set(_deployqt_args ${_cfg_flag} ${_extra_flags} ${_post_build_loc})
    endif(APPLE)

    find_program(DEPLOYQT_EXEC ${_deployqt_exec_name})

    if(NOT DEPLOYQT_EXEC)
        message(AUTHOR_WARNING
            "Failed to locate ${_deployqt_exec_name} executable. Please run \
${_deployqt_exec_name} for target ${TARGET_NAME} manually after build/install.")
        return()
    endif(NOT DEPLOYQT_EXEC)

    get_filename_component(_deployqt_dir ${DEPLOYQT_EXEC} DIRECTORY)

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND ${DEPLOYQT_EXEC} ${_deployqt_args}
        WORKING_DIRECTORY ${_deployqt_dir}
        COMMENT "Deploying Qt for target ${TARGET_NAME}"
        VERBATIM USES_TERMINAL
    )

    if(NOT DQT_INSTALL)
        return()
    endif(NOT DQT_INSTALL)

    if(DQT_COMPONENT)
        set(_inst_comp_spec COMPONENT ${DQT_COMPONENT})
    else(DQT_COMPONENT)
        unset(_inst_comp_spec)
    endif(DQT_COMPONENT)

    string(MD5 _inst_dir_hash ${DQT_INSTALL})
    string(SUBSTRING ${_inst_dir_hash} 0 6 _inst_dir_hash)
    file(GENERATE
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/DeployQt-${TARGET_NAME}-${_inst_dir_hash}-$<CONFIG>.cmake
        CONTENT "
if(NOT IS_ABSOLUTE \"${DQT_INSTALL}\")
    get_filename_component(_inst_dir \"${DQT_INSTALL}\" ABSOLUTE
        BASE_DIR \${CMAKE_INSTALL_PREFIX})
endif(NOT IS_ABSOLUTE \"${DQT_INSTALL}\")
if(APPLE)
    set(_deployqt_args \${_inst_dir} \"${_extra_flags}\")
elseif(WIN32)
    set(_cfg_flag $<$<CONFIG:Release>:--release>$<$<CONFIG:Debug>:--debug>)
    set(_deployqt_args \${_cfg_flag} \"${_extra_flags}\" \${_inst_dir})
endif(APPLE)
execute_process(
    COMMAND \"${DEPLOYQT_EXEC}\" \${_deployqt_args}
    WORKING_DIRECTORY \"${_deployqt_dir}\"
)
"
    )
    install(CODE
        "include(${CMAKE_CURRENT_BINARY_DIR}/DeployQt-${TARGET_NAME}-${_inst_dir_hash}-\${CMAKE_INSTALL_CONFIG_NAME}.cmake)"
        ${_inst_comp_spec}
    )
endfunction(sm_deployqt)

# Delete all CMake related files in the build directory to ensure that,
# when changes are made to cache arguments here, the stale cache values
# will not persist.
function(sm_clean_ext_proj_build_sys_files BINARY_DIR PACKAGE_NAME)
    message("Clean existing ${PACKAGE_NAME} build dir")
    file(GLOB_RECURSE BUILD_SYS_FILES LIST_DIRECTORIES true
        RELATIVE ${BINARY_DIR} ${BINARY_DIR}/*.vcxproj ${BINARY_DIR}/*.sln )
    foreach (RELATED_FILE
        CMakeFiles CMakeTmp CMakeScripts cmake_install.cmake CMakeCache.txt
        CTestCustom.cmake ${BUILD_SYS_FILES})
        message("   Deleting ${BINARY_DIR}/${RELATED_FILE} ...")
        file(REMOVE_RECURSE ${BINARY_DIR}/${RELATED_FILE})
    endforeach (RELATED_FILE)
    message("   Deleting Stamp and tmp directories...")
    file(REMOVE_RECURSE
        ${SM_3RDPARTY_BUILD_DIR}/Stamp/${PACKAGE_NAME}
        ${SM_3RDPARTY_BUILD_DIR}/tmp/${PACKAGE_NAME}
    )
endfunction(sm_clean_ext_proj_build_sys_files)

# Use this function to convert a list of command line style cache initializers
# to a cache preloader script that can be passed to cmake command.
# Each command line style initializer looks like:
# -D<var>:<type>=<value>
#	or
# -D<var>=<value>
# The generated preloader file will be composed of one or more lines looking
# like this:
#
# set(<var> <value> CACHE <type> "..." FORCE)
# set(<var> <value> CACHE <type> "..." FORCE)
# set(<var> <value> CACHE)
# set(<var> <value> CACHE)
# set...
#
# The generated cache preloader file can be passed to the cmake command using:
# cmake -C <initial-cache> ...
function(sm_convert_cmdln_cache_args_to_cache_preloader
    CACHE_PRELOADER_FILE CACHE_ARGS)
    file(WRITE ${CACHE_PRELOADER_FILE} "")

    # The regex to match a single initializer of the form -D<var>:<type>=<value>
    # or -D<var>=<value> with capture groups for var, type, and value
    set(_cache_arg_typed_def_regex "^-D\\s*([a-zA-Z_][0-9a-zA-Z_]*)\
(:(BOOL|FILEPATH|PATH|STRING|INTERNAL))=(.*)")
    set(_cache_arg_typeless_def_regex "^-D\\s*([a-zA-Z_][0-9a-zA-Z_]*)=(.*)")

    # Care must be taken as each of the elements in CACHE_ARGS is not
    # necessarily a cache initializer definition. This is because some cache
    # initializers contain semicolons in their values and the semicolons
    # are interpreted as list separator in CMake. So each item in CACHE_ARGS
    # might be the beginning of an initializer or continuation of that value
    # of the last one.
    foreach(_cache_arg ${CACHE_ARGS})
        string(REGEX MATCH ${_cache_arg_typed_def_regex}
            _matched_arg_typed "${_cache_arg}")
        string(REGEX MATCH ${_cache_arg_typeless_def_regex}
            _matched_arg_typeless "${_cache_arg}")
        if (_matched_arg_typed OR _matched_arg_typeless)
            # Beginning of an initializer
            if (_setter_line_ending)
                # This is not our first initializer. So we have to close the
                # definition for the last one.
                file(APPEND ${CACHE_PRELOADER_FILE} ${_setter_line_ending})
            endif (_setter_line_ending)

            # For typeless initializers, we'll take the type to be STRING.
            # Otherwise we won't be able to define a cache entry.
            if (_matched_arg_typed)
                string(REGEX REPLACE ${_cache_arg_typed_def_regex}
                    "\\1" _cache_arg_name "${_cache_arg}")
                string(REGEX REPLACE ${_cache_arg_typed_def_regex}
                    "\\3" _cache_arg_type "${_cache_arg}")
                string(REGEX REPLACE ${_cache_arg_typed_def_regex}
                    "\\4" _cache_arg_value "${_cache_arg}")
            else (_matched_arg_typed)
                string(REGEX REPLACE ${_cache_arg_typeless_def_regex}
                    "\\1" _cache_arg_name "${_cache_arg}")
                string(REGEX REPLACE ${_cache_arg_typeless_def_regex}
                    "\\2" _cache_arg_value "${_cache_arg}")
                set(_cache_arg_type STRING)
            endif (_matched_arg_typed)
            set(_setter_line_ending " CACHE ${_cache_arg_type} \
\"Generated by sm_convert_cmdln_cache_args_to_cache_preloader\" FORCE)\n")

            # Open new entry definition
            file(APPEND ${CACHE_PRELOADER_FILE}
                "\nset(${_cache_arg_name} \"${_cache_arg_value}\"")
        else (_matched_arg_typed OR _matched_arg_typeless)
            # So this is a continuation of the value of the last initializer.
            # We will just insert the semicolon back in an proceed.
            file(APPEND ${CACHE_PRELOADER_FILE} " ${_cache_arg}")
        endif (_matched_arg_typed OR _matched_arg_typeless)
    endforeach(_cache_arg)

    # Close the last entry definition
    if (_setter_line_ending)
        file(APPEND ${CACHE_PRELOADER_FILE} ${_setter_line_ending})
    endif (_setter_line_ending)

endfunction(sm_convert_cmdln_cache_args_to_cache_preloader)

# Use the sm_include_build_target_cmake_time function to build the
# specified targets within one or more CMake files during processing of
# the current list.
# Syntax:
# sm_include_build_target_cmake_time(CMAKE_MIN_REQ <min-ver>
#		[PROJ_NAME <proj-name>] [PROJ_PATH <proj-path>]
#		MODULES <module1> [<module2> [<module3> ...]]
#		[TARGETS [<target1> [<target2> [<target3> ...]]]]
#		[CACHE_ARGS [-D<var1>:<type1>=<value1> [-D<var1>:<type1>=<value1> ...]]]
#
# The sm_include_build_target_cmake_time command creates a new CMakeLists file
# using the specified input and then proceeds with configuration and building
# of the specified targets immediately.
# CMAKE_MIN_REQ specifies the minimum version to declare using
# cmake_minimum_required for the created CMakeLists file, e.g. 3.9. If
# specified, cmake_minimum_required(VERSION <min-ver>) will be added to the
# created list file.
# PROJ_NAME specifies an optional name for the project. If specified,
# project(<proj-name>) will be added.
# Use PROJ_PATH to specify a directory for the project to be created in and
# built. If ommitted, a random directory within the current binary directory
# is used for the project.
# MODULES names the list of CMake modules (files) to include within the created
# list file using include command. For each of the modules specified, a line
# of the form include(<moduleN>) is added to the created list file. At least
# one module must be passed.
# Use TARGETS to specify which targets to build after the project has been
# created and configured. If ommitted, the default targets are built.
# Use CACHE_ARGS to initialize the CMake cache for the created project using
# command line style initializers. Since the modules are loaded using CMake
# include command, you should probably initialize CMAKE_MODULE_PATH so the
# module(s) can be located.
# Example:
#	sm_include_build_target_cmake_time(
#		CMAKE_MIN_REQ 3.9
#		PROJ_NAME MyProject
#		MODULES InitModule MainModule
#		CACHE_ARGS
#			-DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
#			-DCMAKE_MODULE_PATH:STRING=${CMAKE_MODULE_PATH}
#	)
function(sm_include_build_target_cmake_time)
    cmake_parse_arguments(SM_INC ""
        "CMAKE_MIN_REQ;PROJ_NAME;PROJ_PATH"
        "MODULES;TARGETS;CACHE_ARGS" ${ARGN})

    if(SM_INC_PROJ_NAME)
        set(_project_line "project(${SM_INC_PROJ_NAME})\n")
    endif(SM_INC_PROJ_NAME)
    if(SM_INC_CMAKE_MIN_REQ)
        set(_version_line
            "cmake_minimum_required(VERSION ${SM_INC_CMAKE_MIN_REQ})\n")
    endif(SM_INC_CMAKE_MIN_REQ)
    if(NOT SM_INC_MODULES)
        message(WARNING
        "At least one module is needed by sm_include_build_target_cmake_time")
        return()
    else(NOT SM_INC_MODULES)
        string(REPLACE ";" " " _modules_space_separated "${SM_INC_MODULES}")
    endif(NOT SM_INC_MODULES)

    set(SM_INC_LIST_FILE_CONTENTS
"${_version_line}${_project_line}\
foreach(_module_name ${_modules_space_separated})
    include(\${_module_name})
endforeach(_module_name)
")

    if(NOT SM_INC_PROJ_PATH)
        string(RANDOM LENGTH 8 SM_INC_PROJ_PATH)
    endif(NOT SM_INC_PROJ_PATH)

    file(WRITE ${SM_INC_PROJ_PATH}/CMakeLists.txt ${SM_INC_LIST_FILE_CONTENTS})

    sm_convert_cmdln_cache_args_to_cache_preloader(
        ${SM_INC_PROJ_PATH}/InitCache.txt "${SM_INC_CACHE_ARGS}")

    set(_config_command
        ${CMAKE_COMMAND} -C InitCache.txt -G "${CMAKE_GENERATOR}"
        -DCMAKE_GENERATOR_INSTANCE=${CMAKE_GENERATOR_INSTANCE}
    )

    if(CMAKE_GENERATOR_PLATFORM)
        list(APPEND _config_command -A ${CMAKE_GENERATOR_PLATFORM})
    endif(CMAKE_GENERATOR_PLATFORM)
    if(CMAKE_GENERATOR_PLATFORM)
        list(APPEND _config_command -T ${CMAKE_GENERATOR_TOOLSET})
    endif(CMAKE_GENERATOR_PLATFORM)

    execute_process(COMMAND ${_config_command} .
        WORKING_DIRECTORY ${SM_INC_PROJ_PATH})

    if(SM_INC_TARGETS)
        foreach(_target ${SM_INC_TARGETS})
            execute_process(
                COMMAND ${CMAKE_COMMAND} --build . --target ${_target}
                WORKING_DIRECTORY ${SM_INC_PROJ_PATH})
        endforeach(_target)
    else(SM_INC_TARGETS)
        execute_process(COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${SM_INC_PROJ_PATH})
    endif(SM_INC_TARGETS)
endfunction(sm_include_build_target_cmake_time)

# Output the directory listing for the specified path - useful for debugging
# build on the CI machine.
function(sm_list_dir DIR_PATH)
    file(GLOB  DIR_LIST LIST_DIRECTORIES true RELATIVE ${DIR_PATH} ${DIR_PATH}/*)
    string(REPLACE ";" "\n" DIR_LIST "${DIR_LIST}")
    message("Directory listing for '${DIR_PATH}:'\n${DIR_LIST}\n")
endfunction(sm_list_dir)

# Create a directory if it doesn't exist.
macro(create_dir_if_not_exists DIR_PATH)
    if(NOT EXISTS ${DIR_PATH})
        file(MAKE_DIRECTORY ${DIR_PATH})
    endif(NOT EXISTS ${DIR_PATH})
endmacro(create_dir_if_not_exists)

# Create a text file within the build directory for 3rd-party libraries with
# information about the build system. This file is necessary because the build
# directory name is a shortened hash of all the build system configuration and
# and the configuration is not obvious from directory name.
macro(create_3rdparty_build_info_file)
    create_dir_if_not_exists(${SM_PROGRAMDATA_DIR})
    create_dir_if_not_exists(${SM_PROGRAMDATA_DIR}/build)
    create_dir_if_not_exists(${SM_3RDPARTY_BUILD_DIR})

    if(NOT EXISTS ${SM_BUILD_INFO_FILE})
        file(WRITE ${SM_BUILD_INFO_FILE}
"Build system info
==================
CMAKE_GENERATOR: ${CMAKE_GENERATOR}
CMAKE_GENERATOR_PLATFORM: ${CMAKE_GENERATOR_PLATFORM}
CMAKE_GENERATOR_TOOLSET: ${CMAKE_GENERATOR_TOOLSET}
")
    endif(NOT EXISTS ${SM_BUILD_INFO_FILE})
endmacro(create_3rdparty_build_info_file)

# Determines whether a given path (DESCENDANT_PATH) is a direct or indirect
# descendant of a directory (ANCESTOR_PATH) after resolving symlinks.
function(sm_is_descendant_of_dir RESULT_VAR DESCENDANT_PATH ANCESTOR_PATH)
    get_filename_component(_REAL_ANCESTOR_PATH ${ANCESTOR_PATH} REALPATH)
    get_filename_component(_REAL_DESCENDANT_PATH ${DESCENDANT_PATH} REALPATH)
    get_filename_component(
        _DESCENDANT_PARENT ${_REAL_DESCENDANT_PATH} DIRECTORY)
    if(${_DESCENDANT_PARENT} STREQUAL ${_REAL_ANCESTOR_PATH})
        # Found it
        set(${RESULT_VAR} YES PARENT_SCOPE)
    elseif(${_DESCENDANT_PARENT} STREQUAL ${_REAL_DESCENDANT_PATH})
        # Hit the root directory
        set(${RESULT_VAR} NO PARENT_SCOPE)
    else(${_DESCENDANT_PARENT} STREQUAL ${_REAL_ANCESTOR_PATH})
        # Recurse
        sm_is_descendant_of_dir(
            _RECURSION_RESULT ${_DESCENDANT_PARENT} ${_REAL_ANCESTOR_PATH})
        set(${RESULT_VAR} ${_RECURSION_RESULT} PARENT_SCOPE)
    endif(${_DESCENDANT_PARENT} STREQUAL ${_REAL_ANCESTOR_PATH})
endfunction(sm_is_descendant_of_dir)

# Populate the output variable with the list of interface include directories
# for the given target
function(sm_get_target_iface_include_dirs _target _out_var)
    if(NOT TARGET ${_target})
        return()
    endif(NOT TARGET ${_target})

    get_target_property(iface_inc ${_target} INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(
        iface_sys_inc ${_target} INTERFACE_SYSTEM_INCLUDE_DIRECTORIES)

    set(inc_dir)
    if(iface_inc)
        list(APPEND inc_dir ${iface_inc})
    endif(iface_inc)
    if(iface_sys_inc)
        list(APPEND inc_dir ${iface_sys_inc})
    endif(iface_sys_inc)

    set(${_out_var} ${inc_dir} PARENT_SCOPE)
endfunction(sm_get_target_iface_include_dirs)

option(USE_OLD_FINDPACKAGE
    "Whether to use plain old find_package instead of building packages if not found"
    OFF)

# We shall use this macro to find a package instead of plain find_package.
# Its syntax is identical to that of find_package.
# It calls the find_package and if the package cannot be found, or if the
# found package include directories are not in the SMUtil third party
# install directory, it includes the relevant module for downloading, building,
# and installing of the package.
# Use the VERIF_TARGET to specify an imported target for verifying
# the finding of the package and the location of include directories.
# Use the EXTRA_PREFIX to specify a supplementary path to be appended to the
# default SMUtil third party install directory. It will accept the package
# only if it is found within that directory and, if not found, it will build
# and install it to that directory.
macro(sm_find_or_get PACKAGE_NAME)
if (USE_OLD_FINDPACKAGE)
    find_package(${PACKAGE_NAME} ${SM_FOG_UNPARSED_ARGUMENTS})
else(USE_OLD_FINDPACKAGE)
    cmake_parse_arguments(
        SM_FOG "" "VERIF_TARGET;EXTRA_PREFIX" "" ${ARGN})

    set(_push_CMAKE_FIND_FRAMEWORK ${CMAKE_FIND_FRAMEWORK})
    set(_push_CMAKE_FIND_APPBUNDLE ${CMAKE_FIND_APPBUNDLE})
    set(CMAKE_FIND_FRAMEWORK LAST)
    set(CMAKE_FIND_APPBUNDLE LAST)

    # First we need to see if the package can be found, so it a REQUIRED clause
    # is present, we remove it for the time being so the call doesn't fail.
    set(MODIFIED_ARGS ${SM_FOG_UNPARSED_ARGUMENTS})
    if (NOT QUIET IN_LIST MODIFIED_ARGS)
        list(APPEND MODIFIED_ARGS QUIET)
    endif (NOT QUIET IN_LIST MODIFIED_ARGS)
    list(REMOVE_ITEM MODIFIED_ARGS REQUIRED)

    find_package(${PACKAGE_NAME} ${MODIFIED_ARGS})
    if(SM_FOG_VERIF_TARGET)
        sm_get_target_iface_include_dirs(
            ${SM_FOG_VERIF_TARGET} ${PACKAGE_NAME}_INCLUDE_DIRS)
    endif(SM_FOG_VERIF_TARGET)

    if (SM_FOG_EXTRA_PREFIX)
        set(_acceptable_prefix
            ${SM_3RDPARTY_INSTALL_DIR}/${SM_FOG_EXTRA_PREFIX})
    else (SM_FOG_EXTRA_PREFIX)
        set(_acceptable_prefix ${SM_3RDPARTY_INSTALL_DIR})
    endif (SM_FOG_EXTRA_PREFIX)

    if(${PACKAGE_NAME}_FOUND OR TARGET "${SM_FOG_VERIF_TARGET}")
        list(GET ${PACKAGE_NAME}_INCLUDE_DIRS 0 _first_include_dir)
        sm_is_descendant_of_dir(_correct_package
            ${_first_include_dir} ${_acceptable_prefix})
    else(${PACKAGE_NAME}_FOUND OR TARGET "${SM_FOG_VERIF_TARGET}")
        unset(_correct_package)
    endif(${PACKAGE_NAME}_FOUND OR TARGET "${SM_FOG_VERIF_TARGET}")

    set(PACKAGE_NAME_INCLUDE_DIRS ${${PACKAGE_NAME}_INCLUDE_DIRS})

    if(_correct_package)
        message("SMUtil built ${PACKAGE_NAME} found at:${PACKAGE_NAME_INCLUDE_DIRS}")
    else(_correct_package)
        # So we have to create and build a new project that includes the module
        # with build targets for retrieving, building, and installing of the
        # package.
        message("Unable to find SMUtil ${PACKAGE_NAME}: Try to build ...")
        set(_ext_proj_path ${CMAKE_BINARY_DIR}/3rdparty/${PACKAGE_NAME})
        file(REMOVE_RECURSE ${_ext_proj_path})

        list(GET MODIFIED_ARGS 0 _find_package_ver_arg)
        if (_find_package_ver_arg VERSION_GREATER 0)
            set(_requested_version_cache_setting
                "-DSM_${PACKAGE_NAME}_REQUESTED_VERSION:\
STRING=${_find_package_ver_arg}")
        else (_find_package_ver_arg VERSION_GREATER 0)
            set(_requested_version_cache_setting)
        endif (_find_package_ver_arg VERSION_GREATER 0)

        list(FIND MODIFIED_ARGS EXACT _exact_ver_requested)
        if(_exact_ver_requested)
            set(_exact_ver_requested_cache_setting
                "-DSM_${PACKAGE_NAME}_REQUESTED_EXACT:BOOL=ON")
        else(_exact_ver_requested)
            set(_exact_ver_requested_cache_setting)
        endif(_exact_ver_requested)

        if (SM_FOG_EXTRA_PREFIX)
            set(_set_different_install_prefix
                -DSM_EXTRA_PREFIX=${SM_FOG_EXTRA_PREFIX})
        else (SM_FOG_EXTRA_PREFIX)
            set(_set_different_install_prefix)
        endif (SM_FOG_EXTRA_PREFIX)

        create_3rdparty_build_info_file()
        sm_include_build_target_cmake_time(
            CMAKE_MIN_REQ 3.9
            PROJ_NAME Retrieve${PACKAGE_NAME}
            MODULES SMRetrieve${PACKAGE_NAME}
            PROJ_PATH ${_ext_proj_path}
            CACHE_ARGS
                -DSM_DOWNLOAD_CACHE_DIR=${SM_DOWNLOAD_CACHE_DIR}
                -DSM_3RDPARTY_BUILD_DIR=${SM_3RDPARTY_BUILD_DIR}
                -DSM_3RDPARTY_INSTALL_DIR=${SM_3RDPARTY_INSTALL_DIR}
                -DSM_3RDPARTY_CONFIGURATION_TYPES=${SM_3RDPARTY_CONFIGURATION_TYPES}
                -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
                -DCMAKE_MODULE_PATH:STRING=${CMAKE_MODULE_PATH}
                -DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
                -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
                -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
                ${_set_different_install_prefix}
                ${_requested_version_cache_setting}
        )

        # Now it should be safe to call find_package (with the REQUIRED clause
        # if it was ommitted at first)
        find_package(${PACKAGE_NAME} ${SM_FOG_UNPARSED_ARGUMENTS})
    endif(_correct_package)

    set(CMAKE_FIND_APPBUNDLE ${_push_CMAKE_FIND_APPBUNDLE})
    set(CMAKE_FIND_FRAMEWORK ${_push_CMAKE_FIND_FRAMEWORK})
endif(USE_OLD_FINDPACKAGE)
endmacro(sm_find_or_get)

# If passing a list as the default value, make sure to "qoute" it, Otherwise
# only the first item in the list is taken as the default value and the rest
# are discarded.
macro(sm_set_default_var_value VAR_NAME DEFAULT_VAL)
    if(NOT ${VAR_NAME})
        set(${VAR_NAME} ${DEFAULT_VAL})
    endif(NOT ${VAR_NAME})
endmacro(sm_set_default_var_value)

function(sm_add_ext_proj_patch_step EXT_PROJ_NAME EXT_PROJ_TARGET_NAME)
    set(PATCHED_FILES_DIR ${SM_MODULE_PATH}/patch/${EXT_PROJ_NAME})
    if (EXISTS ${PATCHED_FILES_DIR})
        file(GLOB_RECURSE PATCHED_ITEMS
            LIST_DIRECTORIES true RELATIVE ${PATCHED_FILES_DIR}
            ${PATCHED_FILES_DIR}/*)

        set(PATCH_COMMANDS)
        foreach(PATCHED_ITEM ${PATCHED_ITEMS})
            if(IS_DIRECTORY ${PATCHED_FILES_DIR}/${PATCHED_ITEM})
                list(APPEND PATCH_COMMANDS
                    COMMAND ${CMAKE_COMMAND} -E
                        make_directory ./${PATCHED_ITEM}
                )
            else(IS_DIRECTORY ${PATCHED_FILES_DIR}/${PATCHED_ITEM})
                list(APPEND PATCH_COMMANDS
                    COMMAND ${CMAKE_COMMAND} -E copy
                        ${PATCHED_FILES_DIR}/${PATCHED_ITEM}
                        ${PATCHED_ITEM}
                )
            endif(IS_DIRECTORY ${PATCHED_FILES_DIR}/${PATCHED_ITEM})
        endforeach(PATCHED_ITEM)

        ExternalProject_Add_Step(
            ${EXT_PROJ_TARGET_NAME} copy_patched_files
            COMMENT
                "Copying patched ${EXT_PROJ_NAME} files to source directory..."
            WORKING_DIRECTORY <SOURCE_DIR>
            DEPENDEES update
            DEPENDERS patch
            ${PATCH_COMMANDS}
        )
    endif (EXISTS ${PATCHED_FILES_DIR})
endfunction(sm_add_ext_proj_patch_step)

# The sm_add_ext_cmake_project adds a new target that downloads, extracts,
# builds, and installs a CMake based projects. It uses the ExternalProject
# modules and customizes many of its options and adapts them to our needs.
# It will download from the given URL to the given download directory (or
# the default), compares with the given SHA1 hash, extracts it to the given
# source directory (or the default). If it finds a directory with the same name
# as the project under the patch directory in the same directory as
# SMUtil.cmake, it will copy the contents to the source directory as a
# patch step. It then builds the projects in the given configurations (or
# the defaults). It then installs the project to the given directory (or
# the default). Extra CMake cache initializers may also be given.
# The default configuration:
#   * Debug and Release configurations
#   * Static libraries
#   * Download directory:
#       ${SM_PROGRAMDATA_DIR}/cache/Download/<proj-name>
#   * Source directory:
#       ${SM_PROGRAMDATA_DIR}/cache/Source/<proj-name>
#   * Binary directory: ${CMAKE_BINARY_DIR}/Build/<proj-name>
#   * Install prefix: ${SM_PROGRAMDATA_DIR}(64)?
#   * Debug libraries postfixed with `d`
# The following CMake variables are also passed by default to the build
# environment:
#   * CMAKE_PREFIX_PATH
#   * CMAKE_OSX_DEPLOYMENT_TARGET
## Syntax:
#	sm_add_ext_cmake_project(<proj-name>
#		URL <url> SHA1 <sha1> [DOWNLOAD_DIR <dl-dir>] [SOURCE_DIR <src-dir>]
#		[BINARY_DIR <build-dir>] [INSTALL_DIR <inst-dir>]
#		[CONFIGS <Debug|Release|...> ...] [PATCH_COMMAND <patch-cmd>]
#		[EXTRA_CACHE_ARGS
#			[-D<var1>:<type1>=<value1> [-D<var1>:<type1>=<value1> ...]]]
#		[EXTRA_CACHE_ARGS_APPLE
#			[-D<var1>:<type1>=<value1> [-D<var1>:<type1>=<value1> ...]]]
#		[EXTRA_CACHE_ARGS_WIN32
#			[-D<var1>:<type1>=<value1> [-D<var1>:<type1>=<value1> ...]]]
#	)
# Example:
#	sm_add_ext_cmake_project(
#		VTK7
#		URL https://github.com/Kitware/VTK/archive/v7.1.1.tar.gz
#		SHA1 7b60d17db0214de56f6bac73122952f9cbcdc7b2
#		EXTRA_CACHE_ARGS
#			-DVTK_USE_SYSTEM_TIFF:BOOL=ON
#			-DVTK_USE_SYSTEM_ZLIB:BOOL=ON
#	)
function(sm_add_ext_cmake_project EXT_PROJ_NAME)
    set(OPTION_KEYWORDS "")
    set(SINGLE_VALUE_KEYWORDS
        "URL;SHA1;DOWNLOAD_DIR;SOURCE_DIR;BINARY_DIR;\
INSTALL_DIR;SOURCE_SUBDIR;PDB_INSTALL_DIR")
    set(MULTI_VALUE_KEYWORDS
        "CONFIGS;EXTRA_CACHE_ARGS;EXTRA_CACHE_ARGS_APPLE;\
EXTRA_CACHE_ARGS_WIN32;PATCH_COMMAND"
    )
    cmake_parse_arguments(
        EXT_PROJ
        "${OPTION_KEYWORDS}"
        "${SINGLE_VALUE_KEYWORDS}"
        "${MULTI_VALUE_KEYWORDS}"
        ${ARGN}
    )

    string(TOLOWER ${EXT_PROJ_NAME} _ext_proj_name)

    sm_set_default_var_value(EXT_PROJ_DOWNLOAD_DIR
        ${SM_DOWNLOAD_CACHE_DIR}/Download/${_ext_proj_name})

    sm_set_default_var_value(EXT_PROJ_SOURCE_DIR
        ${SM_DOWNLOAD_CACHE_DIR}/Source/${_ext_proj_name})

    sm_set_default_var_value(EXT_PROJ_SOURCE_SUBDIR .)

    sm_set_default_var_value(
        EXT_PROJ_BINARY_DIR ${SM_3RDPARTY_BUILD_DIR}/${_ext_proj_name})

    if (SM_EXTRA_PREFIX)
        set(_default_install_prefix
            ${SM_3RDPARTY_INSTALL_DIR}/${SM_EXTRA_PREFIX})
    else (SM_EXTRA_PREFIX)
        set(_default_install_prefix ${SM_3RDPARTY_INSTALL_DIR})
    endif (SM_EXTRA_PREFIX)
    sm_set_default_var_value(
        EXT_PROJ_INSTALL_DIR ${_default_install_prefix})

    sm_set_default_var_value(
        EXT_PROJ_CONFIGS "${SM_3RDPARTY_CONFIGURATION_TYPES}")

    if(TARGET ${_ext_proj_name})
        message(AUTHOR_WARNING
            "Target ${_ext_proj_name} already exists. Giving up...")
        return()
    endif(TARGET ${_ext_proj_name})
    sm_clean_ext_proj_build_sys_files(${EXT_PROJ_BINARY_DIR} ${_ext_proj_name})

    # Append platform specific extra cache arguments:
    if(APPLE)
        list(APPEND EXT_PROJ_EXTRA_CACHE_ARGS
            ${EXT_PROJ_EXTRA_CACHE_ARGS_APPLE})
    elseif(WIN32)
        list(APPEND EXT_PROJ_EXTRA_CACHE_ARGS
            ${EXT_PROJ_EXTRA_CACHE_ARGS_WIN32})
    endif(APPLE)

    set(_pdb_output_dir <BINARY_DIR>/PDBFilesDebug)
    if (MSVC AND EXT_PROJ_PDB_INSTALL_DIR)
        set(_specify_pdb_output_dir
            -DCMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG:PATH=${_pdb_output_dir}
        )
    else (MSVC AND EXT_PROJ_PDB_INSTALL_DIR)
        set(_specify_pdb_output_dir)
    endif (MSVC AND EXT_PROJ_PDB_INSTALL_DIR)

    # The URL HASH created using online facility:
    # 	https://hash.online-convert.com/sha1-generator
    ExternalProject_Add(
        ${_ext_proj_name}
        URL ${EXT_PROJ_URL}
        URL_HASH SHA1=${EXT_PROJ_SHA1}
        DOWNLOAD_DIR ${EXT_PROJ_DOWNLOAD_DIR}
        SOURCE_DIR ${EXT_PROJ_SOURCE_DIR}
        BINARY_DIR ${EXT_PROJ_BINARY_DIR}
        INSTALL_DIR ${EXT_PROJ_INSTALL_DIR}
        SOURCE_SUBDIR ${EXT_PROJ_SOURCE_SUBDIR}
        PATCH_COMMAND ${EXT_PROJ_PATCH_COMMAND}
        BUILD_COMMAND ""	# Multiple custom commands are added instead for
        INSTALL_COMMAND ""	# each platform configuration
        CMAKE_CACHE_DEFAULT_ARGS
            -DCMAKE_INSTALL_PREFIX:STRING=<INSTALL_DIR>
            ${_specify_pdb_output_dir}
            -DBUILD_SHARED_LIBS:BOOL=OFF
            -DBUILD_EXAMPLES:BOOL=OFF
            -DBUILD_TESTING:BOOL=OFF
            -DCMAKE_C_COMPILER:PATH=${CMAKE_C_COMPILER}
            -DCMAKE_CXX_COMPILER:PATH=${CMAKE_CXX_COMPILER}
            "-DCMAKE_CONFIGURATION_TYPES:STRING=${EXT_PROJ_CONFIGS}"
            "-DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}"
            -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
            ${EXT_PROJ_EXTRA_CACHE_ARGS}
    )

    sm_add_ext_proj_patch_step(${EXT_PROJ_NAME} ${_ext_proj_name})

    if(APPLE)
        set(EXTRA_BUILDTOOL_FLAGS)
    elseif(MSVC AND NOT _specify_pdb_output_dir)
        set(EXTRA_BUILDTOOL_FLAGS
            /m  # MSBuild parallel build
        )
    endif(APPLE)

    foreach(_build_cfg ${EXT_PROJ_CONFIGS})
        ExternalProject_Add_Step(${_ext_proj_name} build_${_build_cfg}
            COMMENT "Building ${EXT_PROJ_NAME} in configuration ${_build_cfg}"
            COMMAND ${CMAKE_COMMAND} --build <BINARY_DIR>
                --config ${_build_cfg} -- ${EXTRA_BUILDTOOL_FLAGS}
            DEPENDEES configure
            DEPENDERS build
        )
        ExternalProject_Add_Step(${_ext_proj_name} install_${_build_cfg}
            COMMENT "Installing ${EXT_PROJ_NAME} in configuration ${_build_cfg}"
            COMMAND ${CMAKE_COMMAND}
                --build <BINARY_DIR> --target install --config ${_build_cfg}
            DEPENDEES build
            DEPENDERS install
        )
    endforeach(_build_cfg)

    if (MSVC AND EXT_PROJ_PDB_INSTALL_DIR)
        if (NOT IS_ABSOLUTE ${EXT_PROJ_PDB_INSTALL_DIR})
            set(EXT_PROJ_PDB_INSTALL_DIR
                <INSTALL_DIR>/${EXT_PROJ_PDB_INSTALL_DIR})
        endif (NOT IS_ABSOLUTE ${EXT_PROJ_PDB_INSTALL_DIR})
        ExternalProject_Add_Step(${_ext_proj_name} install_pdbs
            COMMENT "Installing ${EXT_PROJ_NAME} debug PDBs"
            COMMAND ${CMAKE_COMMAND} -E
                copy_directory ${_pdb_output_dir} ${EXT_PROJ_PDB_INSTALL_DIR}
            DEPENDEES build
            DEPENDERS install
        )
    endif (MSVC AND EXT_PROJ_PDB_INSTALL_DIR)

endfunction(sm_add_ext_cmake_project)

function(sm_clean_3rdparty_install_dir)
    message("Clean up 3rdparty library install directory")
    sm_list_dir(${SM_3RDPARTY_INSTALL_DIR})
    set(ALLOWED_DIR_LIST
        .DS_Store
        7-Zip
        Qt5.8.0
        Qt5.9.1
        Qt5.11.1
        Qt5.9.1
        Qt5.12.1
        SDL2.framework
        boost_1_60_0
        bin
        build
        cache
        cmake
        include
        junk
        lib
        metakit-2.4.9.8
        pbourke
        poly2tri
        share
        smopapp
        metakit-2.4.9.8
        cldr-license.txt
        gettext-license.txt
        iconv-license.txt
    )

    set(JUNK_DIR ${SM_3RDPARTY_INSTALL_DIR}/junk)

    file(GLOB EXISTING_DIR_LIST LIST_DIRECTORIES true
        ${SM_3RDPARTY_INSTALL_DIR}/*)

    foreach(DIR_ENTRY ${EXISTING_DIR_LIST})
        get_filename_component(ENTRY_NAME ${DIR_ENTRY} NAME)
        if(NOT ${ENTRY_NAME} IN_LIST ALLOWED_DIR_LIST)
            create_dir_if_not_exists(${JUNK_DIR})
            if(EXISTS ${JUNK_DIR}/${ENTRY_NAME})
                message("Removing ${ENTRY_NAME} already in the junk directory")
                file(REMOVE_RECURSE ${JUNK_DIR}/${ENTRY_NAME})
            endif(EXISTS ${JUNK_DIR}/${ENTRY_NAME})
            message("Moving ${DIR_ENTRY} to junk directory")
            file(RENAME ${DIR_ENTRY} ${JUNK_DIR}/${ENTRY_NAME})
        endif(NOT ${ENTRY_NAME} IN_LIST ALLOWED_DIR_LIST)
    endforeach(DIR_ENTRY ${EXISTING_DIR_LIST})
endfunction(sm_clean_3rdparty_install_dir)

# Use this macro to create a portable command line (usable with
# add_custom_command, etc) that invokes extensions to cmake command
# line tool. These commands are supposed to complement "CMake command line tool"
# (cmake -E). Always make the command VERBATIM to avoid unescaped semicolons
# on Unix.
# Usage:
# sm_create_extended_cmdln(<output-cmdln-var> <ext-cmd-name> <cmd-args> ...)
function(sm_create_extended_cmdln COMMAND_VAR COMMAND_NAME)
    string(REPLACE ";" "$<SEMICOLON>" ESCAPED_ARGS "${ARGN}")
    set(${COMMAND_VAR}
        ${CMAKE_COMMAND}
            -D EXT_COMMAND=${COMMAND_NAME}
            -D CMDARGS=${ESCAPED_ARGS}
            -P ${SM_MODULE_PATH}/CommandLineTool.cmake
        PARENT_SCOPE
    )
endfunction(sm_create_extended_cmdln)

# Append items to the list if the given condition is true
macro(sm_append_list_if CONDITION LISTVAR)
    if(${CONDITION})
        list(APPEND ${LISTVAR} ${ARGN})
    endif(${CONDITION})
endmacro(sm_append_list_if)

# Have the output of a target or a file digitally signed using respective
# tools for the current build platform. If COMMAND_VAR is specified, the
# specified variable will contain the commands needed for signing the target
# output or file. If the COMMAND_VAR var is not specified and the target is
# defined, the command is added to its post-build events.
function(sm_digitally_sign _target)

    cmake_parse_arguments(SM_SGN "MACHINE_STORE;FORCE;DEEP"
        "TIME_STAMP_SEVER;TIME_DIGEST_ALG;FILE_DIGEST_ALG;CERTIFICATE_SHA1;\
SIGNING_IDENTITY;COMMAND_VAR"
        "" ${ARGN})

    set(_filepath ${_target})

    if(MSVC)
        find_program(SM_SIGNTOOL_PATH signtool
            PATHS
                "${SM_PROGRAMFILES}/Windows Kits/10/bin/${SM_ARCHITECTURE}"
                "${SM_PROGRAMFILESX86}/Windows Kits/10/bin/${SM_ARCHITECTURE}"
            DOC "Path to Sign Tool command line utility (part of MSSDK)"
        )
        if(NOT SM_SIGNTOOL_PATH)
            message(FATAL_ERROR "Unable to locate SignTool.exe please add
the containig directory to system path or manually set SM_SIGNTOOL_PATH")
        endif(NOT SM_SIGNTOOL_PATH)
        set(_sign_command ${SM_SIGNTOOL_PATH} sign)
        sm_append_list_if(SM_SGN_FILE_DIGEST_ALG _sign_command
            /fd ${SM_SGN_FILE_DIGEST_ALG})
        sm_append_list_if(SM_SGN_CERTIFICATE_SHA1 _sign_command
            /sha1 ${SM_SGN_CERTIFICATE_SHA1})
        sm_append_list_if(SM_SGN_MACHINE_STORE _sign_command /sm)
        if(SM_SGN_TIME_STAMP_SEVER)
            list(APPEND _sign_command /tr ${SM_SGN_TIME_STAMP_SEVER})
            if(SM_SGN_TIME_DIGEST_ALG)
                list(APPEND _sign_command /td ${SM_SGN_TIME_DIGEST_ALG})
            endif(SM_SGN_TIME_DIGEST_ALG)
        endif(SM_SGN_TIME_STAMP_SEVER)

        if(TARGET ${_target})
            set(_filepath $<TARGET_FILE:${_target}>)
        endif(TARGET ${_target})

    elseif(APPLE)

        find_program(SM_CODESIGN_PATH codesign
            DOC "Path to codesign command line utility (part of MSSDK)")
        if(NOT SM_CODESIGN_PATH)
            message(FATAL_ERROR "Unable to locate codesign please add
the containig directory to system path or manually set SM_CODESIGN_PATH")
        endif(NOT SM_CODESIGN_PATH)

        set(_sign_command ${SM_CODESIGN_PATH})

        sm_append_list_if(SM_SGN_DEEP _sign_command --deep)
        sm_append_list_if(SM_SGN_FORCE _sign_command -f)
        if(SM_SGN_SIGNING_IDENTITY)
            list(APPEND _sign_command -s "${SM_SGN_SIGNING_IDENTITY}")
        else(SM_SGN_SIGNING_IDENTITY)
            message(FATAL_ERROR
                "sm_sign_target_output must be called with SIGNING_IDENTITY
set to a valid signing identity in a keychain or certificate SHA1.")
        endif(SM_SGN_SIGNING_IDENTITY)

        if(TARGET ${_target})
            set(_filepath $<TARGET_BUNDLE_DIR:${_target}>)
        endif(TARGET ${_target})

    else(MSVC)

        message(FATAL_ERROR "Digital signing not supported for this platform.")

    endif(MSVC)

    list(APPEND _sign_command ${_filepath})
    sm_create_extended_cmdln(_error_command fatal_error
        "Tried to sign target ${_target} 3 times but failed each time.")
    set(_sign_command ${_sign_command} || ${_sign_command} || ${_sign_command}
        || ${_error_command})

    if (SM_SGN_COMMAND_VAR)
        set(${SM_SGN_COMMAND_VAR} ${_sign_command} PARENT_SCOPE)
    elseif (TARGET ${_target})
        add_custom_command(TARGET ${_target} POST_BUILD
            COMMAND ${_sign_command}
            VERBATIM USES_TERMINAL
        )
    endif (SM_SGN_COMMAND_VAR)

endfunction(sm_digitally_sign)

function(sm_wrap_single_wxrc _filename)
    set(options VERBOSE EXTRA_CPP_CODE CPP_CODE PYTHON_CODE GETEXT
        VALIDATE VAIDATE_ONLY)
    set(oneValueArgs FUNCTION OUTPUT XRC_SCHEMA)
    set(multiValueArgs DEPENDS)
    cmake_parse_arguments(SMWXRC
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(CMAKE_INCLUDE_CURRENT_DIR ON PARENT_SCOPE)
    find_program(WXRC_EXEC wxrc)
    if(NOT WXRC_EXEC)
        message(FATAL_ERROR "wxrc not found! Reinstall wxWidgets utils")
    endif(NOT WXRC_EXEC)

    get_filename_component(_filename ${_filename} ABSOLUTE)
    set(_wxrc_cmd ${WXRC_EXEC})

    sm_append_list_if(SMWXRC_VERBOSE _wxrc_cmd -v)
    sm_append_list_if(SMWXRC_XRC_SCHEMA _wxrc_cmd
        --xrc-schema=${SMWXRC_XRC_SCHEMA})

    if(SMWXRC_CPP_CODE)
        list(APPEND _wxrc_cmd -c)
        set(_outfile_ext .cpp)
    elseif(SMWXRC_PYTHON_CODE)
        list(APPEND _wxrc_cmd -p)
        set(_outfile_ext .py)
    elseif(SMWXRC_GETEXT)
        list(APPEND _wxrc_cmd -g)
        set(_outfile_ext .txt)
    else(SMWXRC_CPP_CODE)
        set(_outfile_ext .xrs)
    endif(SMWXRC_CPP_CODE)

    sm_append_list_if(SMWXRC_VALIDATE _wxrc_cmd --validate)
    sm_append_list_if(SMWXRC_VAIDATE_ONLY _wxrc_cmd --validate-only)
    sm_append_list_if(SMWXRC_FUNCTION _wxrc_cmd -n ${SMWXRC_FUNCTION})
    sm_set_default_var_value(SMWXRC_OUTPUT XrcRes${_outfile_ext})
    list(APPEND _wxrc_cmd -o ${SMWXRC_OUTPUT})
    set(_output_file ${SMWXRC_OUTPUT})

    if(SMWXRC_EXTRA_CPP_CODE)
        list(APPEND _wxrc_cmd -e)
        get_filename_component(_outfile_base ${_output_file} NAME_WE)
        set(_out_header ${_outfile_base}.h)
    endif(SMWXRC_EXTRA_CPP_CODE)

    list(APPEND _wxrc_cmd ${_filename})

    add_custom_command(
        OUTPUT ${_output_file} ${_out_header}
        COMMAND ${_wxrc_cmd}
        DEPENDS ${_filename} ${SMWXRC_DEPENDS}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT Compiling xrc resource file: ${_filename}
        VERBATIM USES_TERMINAL
    )
endfunction(sm_wrap_single_wxrc)

# If the generator is MSVC, it finds the corresponding vcvarsall.bat and
# stores the path in the output variable. It also appends appropriate arguments
# to the output variable according to the specified ARCH (target architecture)
# and HOST_ARCH (host architecture). If architecture is not specified, current
# architecture will be used for both host and target. Pass the STRING option
# to have the output as a single command line string rather than the usual
# ;-list used in cmake COMMAND arguments.
function(sm_find_vcvarsall _outvar)
    if(NOT MSVC)
        message(AUTHOR_WARNING "sm_find_vcvarsall only works with MSVC!")
        return()
    endif(NOT MSVC)

    set(options REQUIRED STRING)
    set(oneValueArgs ARCH HOST_ARCH)
    set(multiValueArgs)
    cmake_parse_arguments(SMF
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(SM_IS_64_BIT)
        set(_current_arch amd64)
    else(SM_IS_64_BIT)
        set(_current_arch x86)
    endif(SM_IS_64_BIT)

    get_filename_component(_vc_linker_dir ${CMAKE_LINKER} DIRECTORY)
    find_file(_vcvarsall_bat vcvarsall.bat
        ${_vc_linker_dir}/..
        ${_vc_linker_dir}/../..
        # MSVS2017 Community:
        ${_vc_linker_dir}/../../../../../../Auxiliary/Build
        # MSVS2017 Enterprise, etc:
        ${_vc_linker_dir}/../../../../../Auxiliary/Build
    )

    sm_set_default_var_value(SMF_ARCH ${_current_arch})
    sm_set_default_var_value(SMF_HOST_ARCH ${_current_arch})
    if(${SMF_ARCH} STREQUAL ${SMF_HOST_ARCH})
        set(_vcvarsall_arg ${SMF_ARCH})
    else(${SMF_ARCH} STREQUAL ${SMF_HOST_ARCH})
        set(_vcvarsall_arg ${SMF_HOST_ARCH}_${SMF_ARCH})
    endif(${SMF_ARCH} STREQUAL ${SMF_HOST_ARCH})

    if(SMF_STRING)
        string(REPLACE ";" " " _vcvarsall_arg ${_vcvarsall_arg})
        file(TO_NATIVE_PATH ${_vcvarsall_bat} _vcvarsall_bat)
        set(_vcvars_cmd "CALL \"${_vcvarsall_bat}\" ${_vcvarsall_arg}")
    else(SMF_STRING)
        set(_vcvars_cmd CALL ${_vcvarsall_bat} ${_vcvarsall_arg})
    endif(SMF_STRING)

    message("_vcvarsall_bat: ${_vcvarsall_bat}")
    if(_vcvarsall_bat)
        set(${_outvar} ${_vcvars_cmd} PARENT_SCOPE)
    else(_vcvarsall_bat)
        if (SMF_REQUIRED)
            message(FATAL_ERROR "Failed to locate vcvarsall.bat")
        endif ()
        unset(${_outvar} PARENT_SCOPE)
    endif(_vcvarsall_bat)
endfunction(sm_find_vcvarsall)

# Find the AWSCLI utility in the system path or issue an error if not found.
function(sm_find_awscli _outvar)
    find_program(SM_AWS_PATH aws
        DOC "Path to Amazon Web Services Command Line Interface utility")
    if(NOT SM_AWS_PATH)
        message(WARNING "Unable to locate awscli utility. please add \
the containig directory to system path or manually set SM_AWS_PATH")
    endif(NOT SM_AWS_PATH)
    set(${_outvar} ${SM_AWS_PATH} PARENT_SCOPE)
endfunction(sm_find_awscli)

# Add commands for uploading the specified local file to the specified
# S3 URI to the special target upload-aws. It also prints out the Download
# link for each uploaded file/dir (must be specified in the args).
# extra options to aws s3 cp could be appended. Generator expressions can
# be used in LOCAL_PATH.
function(sm_upload_to_aws LOCAL_PATH REMOTE_DIR DL_LINK)
    sm_find_awscli(_aws_path)

    if (NOT _aws_path)
        return()
    endif()

    set(_upload_command
        ${_aws_path} s3 cp ${LOCAL_PATH} ${REMOTE_DIR} ${ARGN})
    set(_success_command ${CMAKE_COMMAND} -E echo
        "Successfully uploaded ${LOCAL_PATH}. Download link: ${DL_LINK}")
    string(MD5 _local_path_hash ${LOCAL_PATH})
    string(SUBSTRING ${_local_path_hash} 0 6 _local_path_hash)
    set(_upload_target_name upload-aws-${_local_path_hash})

    add_custom_target(${_upload_target_name}
        COMMAND ${_upload_command}
        COMMAND ${_success_command}
        COMMENT "Uploading ${LOCAL_PATH} to AWS S3..."
        DEPENDS ${LOCAL_PATH}
        VERBATIM USES_TERMINAL
    )

    add_dependencies(upload-aws ${_upload_target_name})
endfunction(sm_upload_to_aws)

# Add command for creating a disk image file using hdiutil on macOS. Pass The
# output filepath as OUTFILE. Pass extra options to hdiutil using OPTIONS.
# Add file level dependencies using DEPENDS. Generator expressions are
# allowed in DEPENDS and OPTIONS and OUTFILE.
function(sm_hdiutil_create_dmg OUTFILE)
    set(options )
    set(oneValueArgs )
    set(multiValueArgs DEPENDS OPTIONS)
    cmake_parse_arguments(HCD
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    find_program(_hdiutil hdiutil)
    if(NOT _hdiutil)
        message(FATAL_ERROR "Unable to find hdiutil!")
    endif(NOT _hdiutil)

    add_custom_command(
        OUTPUT ${OUTFILE}
        COMMAND ${_hdiutil} create ${HCD_OPTIONS} ${OUTFILE}
        DEPENDS ${HCD_DEPENDS}
        COMMENT "Creating disk image ${OUTFILE} using hdiutil..."
        VERBATIM USES_TERMINAL
    )
endfunction(sm_hdiutil_create_dmg)

# Add command for creating a 7-Zip archive file. Pass The archive filepath
# as OUTFILE. Pass extra switches to 7-Zip using OPTIONS.
# Add file level dependencies using DEPENDS. Generator expressions are
# allowed in DEPENDS and OPTIONS and OUTFILE. Use th SFX_MODULE parameters
# to specify full path to a 7z*.sfx module file. Along with SFX_MODULE,
# if SFX_CONFIG_FILE is specified, the file will be used to generate an
# embedded installer config for the 7-Zip SFX. See 7-Zip documentation for
# details on creating standalone installers using the 7-Zip extra
# modules (v9.20). Generator expressions and ${} style variable expansion
# inside the installer config file are allowed.
function(sm_create_7zip_archive OUTFILE)
    set(options )
    set(oneValueArgs SFX_MODULE SFX_CONFIG_FILE)
    set(multiValueArgs FILES DEPENDS OPTIONS POST_CREATE_COMMAND)
    cmake_parse_arguments(C7A
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(_archive_file ${OUTFILE})
    set(_specify_post_create_command)

    if (C7A_SFX_MODULE)
        set(_archive_file ${OUTFILE}.7z)
        if (C7A_SFX_CONFIG_FILE)
            get_filename_component(
                _cfg_file_realpath ${C7A_SFX_CONFIG_FILE} REALPATH)
            get_filename_component(
                _cfg_file_name ${C7A_SFX_CONFIG_FILE} NAME)
            string(MD5 _config_file_path_hash ${_cfg_file_realpath})
            string(SUBSTRING
                ${_config_file_path_hash} 0 6 _config_file_path_hash)
            set(_config_file_out_base
${CMAKE_CURRENT_BINARY_DIR}/${_cfg_file_name}-${_config_file_path_hash})
            set(_config_file_output ${_config_file_out_base}.txt)
            set(_config_file_gen_output ${_config_file_out_base}-$<CONFIG>.txt)
            set(AT_SIGN @)
            configure_file(${C7A_SFX_CONFIG_FILE} ${_config_file_output})
            file(GENERATE
                OUTPUT ${_config_file_gen_output} INPUT ${_config_file_output})
        endif (C7A_SFX_CONFIG_FILE)
        sm_create_extended_cmdln(_concat_command concat_files ${OUTFILE}
            ${C7A_SFX_MODULE} ${_config_file_gen_output} ${_archive_file})
        list(APPEND _specify_post_create_command COMMAND ${_concat_command})
    endif (C7A_SFX_MODULE)

    if (C7A_POST_CREATE_COMMAND)
        list(APPEND _specify_post_create_command
            COMMAND ${C7A_POST_CREATE_COMMAND})
    endif (C7A_POST_CREATE_COMMAND)

    sm_find_or_get(SevenZip REQUIRED)

    add_custom_command(
        OUTPUT ${OUTFILE}
        COMMAND ${CMAKE_COMMAND} -E remove -f ${_archive_file}
        COMMAND ${SevenZip_7z} a ${C7A_OPTIONS} --
            ${_archive_file} ${C7A_FILES}
        ${_specify_post_create_command}
        DEPENDS ${C7A_DEPENDS}
        COMMENT "Creating 7-Zip archive ${OUTFILE} ..."
        VERBATIM USES_TERMINAL
    )
endfunction(sm_create_7zip_archive)

# Removes duplicate libraries from the list specified by variable LIBS_VAR.
# It behaves differently than list(REMOVE_DUPLICATES) in that it allows
# multiple occurences of special keywords "optimized", "debug", and "general"
# which normally appear in a list of libraries
function(sm_remove_duplicate_libs LIBS_VAR)
    set(_keywords debug general optimized)
    set(_out_libs)
    set(_in_libs ${${LIBS_VAR}})
    foreach(lib ${_in_libs})
        if(NOT lib IN_LIST _out_libs OR lib IN_LIST _keywords)
            list(APPEND _out_libs ${lib})
        else(NOT lib IN_LIST _out_libs OR lib IN_LIST _keywords)
            list(GET _out_libs -1 _last_item)
            if(_last_item IN_LIST _keywords)
                list(REMOVE_AT _out_libs -1)
            endif(_last_item IN_LIST _keywords)
        endif(NOT lib IN_LIST _out_libs OR lib IN_LIST _keywords)
    endforeach(lib)
    set(${LIBS_VAR} ${_out_libs} PARENT_SCOPE)
endfunction(sm_remove_duplicate_libs)

# Removes items from a list of libraries. It behaves differently than
# list(REMOVE_ITEM) in that it has sepcial consideration for the keywords
# "optimized", "debug", and "general" which normally appear in a list of
# libraries. If one of the libraries removed is preceded by one of the
# keywords, the preceding keywords is removed as well.
function(sm_remove_items_libs_list LIBS_VAR)
    set(_in_libs ${${LIBS_VAR}})
    set(_out_libs)
    set(_items_to_remove ${ARGN})
    set(_keywords debug general optimized)
    list(REMOVE_ITEM _items_to_remove ${_keywords})
    foreach(lib ${_in_libs})
        if(NOT lib IN_LIST _items_to_remove OR lib IN_LIST _keywords)
            list(APPEND _out_libs ${lib})
        else(NOT lib IN_LIST _items_to_remove OR lib IN_LIST _keywords)
            list(GET _out_libs -1 _last_item)
            if(_last_item IN_LIST _keywords)
                list(REMOVE_AT _out_libs -1)
            endif(_last_item IN_LIST _keywords)
        endif(NOT lib IN_LIST _items_to_remove OR lib IN_LIST _keywords)
    endforeach(lib)
    set(${LIBS_VAR} ${_out_libs} PARENT_SCOPE)
endfunction(sm_remove_items_libs_list)

# Use this function instead of plain cmake install. It behaves just like
# cmake install (unless the POST_BUILD_ONLY option is specified). Besides
# installing everything using cmake install command, it will also apply The
# installation steps for the output directory of the specified target (using
# POST_BUILD_TARGET). This is useful for development and debugging from
# the build directory. Using this, all the necessary resources, libs, etc
# which are supposed to be installed, are also copied to the output directory.
# The copy takes place as a post-build command of the specified target.
# Use the POST_BUILD_ONLY option if you want the install steps be performed
# as post-build command (and not installed).
# Also on the contrary to the plain cmake install command, sm_install allows
# you to use generator expression within a CODE clause.
function(sm_install)
    set(_post_build_options POST_BUILD_ONLY)
    set(_post_build_oneValueArgs POST_BUILD_TARGET)
    set(_post_build_multiValueArgs )
    cmake_parse_arguments(PB
        "${_post_build_options}"
        "${_post_build_oneValueArgs}"
        "${_post_build_multiValueArgs}"
        ${ARGN}
    )
    set(_install_args ${PB_UNPARSED_ARGUMENTS})

    set(options )
    set(oneValueArgs DESTINATION CODE)
    set(multiValueArgs TARGETS FILES DIRECTORY)
    cmake_parse_arguments(PB
        "${options}" "${oneValueArgs}" "${multiValueArgs}" ${_install_args})

    if(NOT PB_POST_BUILD_ONLY)
        install(${_install_args})
        if(PB_TARGETS)
            add_dependencies(deploy-all ${PB_TARGETS})
        endif(PB_TARGETS)
    endif(NOT PB_POST_BUILD_ONLY)

    if(PB_POST_BUILD_TARGET)
        get_target_property(_tgt_bin_dir ${PB_POST_BUILD_TARGET} BINARY_DIR)
        string(MD5 _inst_id "${_install_args}")
        string(SUBSTRING ${_inst_id} 0 8 _inst_id)
        set(_script_file ${_tgt_bin_dir}/post-build-$<CONFIG>-${_inst_id}.cmake)
        set(_script_contents "")

        if(IS_ABSOLUTE ${PB_DESTINATION})
            file(RELATIVE_PATH
                _relative_inst_dir ${CMAKE_INSTALL_PREFIX} ${PB_DESTINATION})
        else(IS_ABSOLUTE ${PB_DESTINATION})
            set(_relative_inst_dir ${PB_DESTINATION})
        endif(IS_ABSOLUTE ${PB_DESTINATION})

        get_target_property(_is_bundle ${PB_POST_BUILD_TARGET} MACOSX_BUNDLE)
        if(WIN32)
            set(_pb_prefix $<TARGET_FILE_DIR:${PB_POST_BUILD_TARGET}>)
        elseif(APPLE AND _is_bundle)
            set(_pb_prefix $<TARGET_BUNDLE_DIR:${PB_POST_BUILD_TARGET}>/..)
        endif(WIN32)

        if(PB_FILES OR PB_DIRECTORY)
            string(REPLACE ";" " " _files_space_sep "${PB_FILES}")
            string(REPLACE ";" " " _dirs_space_sep "${PB_DIRECTORY}")
            string(APPEND _script_contents "
file(COPY ${_files_space_sep} ${_dirs_space_sep}
    DESTINATION ${_pb_prefix}/${_relative_inst_dir})"
            )
        endif(PB_FILES OR PB_DIRECTORY)
        if(PB_CODE)
            string(APPEND _script_contents "\n${PB_CODE}")
        endif(PB_CODE)

        file(GENERATE OUTPUT ${_script_file} CONTENT "${_script_contents}")
        add_custom_command(TARGET ${PB_POST_BUILD_TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -P ${_script_file}
            WORKING_DIRECTORY ${_tgt_bin_dir}
            VERBATIM USES_TERMINAL
        )
    endif(PB_POST_BUILD_TARGET)
endfunction(sm_install)

# Use this function instead of plain add_tests or gtest_add_tests. This
# ensures that all test targets are added to the dependencies of the deploy
# target (which runs both ctest and cpack). Also using this function will
# make it easier if we need to change the mechanism by which we add tests
# in future.
function(sm_add_tests)
    set(_options )
    set(_oneValueArgs TARGET)
    set(_multiValueArgs )
    cmake_parse_arguments(
        AT "${_options}" "${_oneValueArgs}" "${_multiValueArgs}" ${ARGN})

    set(_skip_dep)
    if (NOT (SKIP_DEPENDENCY IN_LIST ARGN))
        set(_skip_dep SKIP_DEPENDENCY)
    endif (NOT (SKIP_DEPENDENCY IN_LIST ARGN))
    gtest_add_tests(${ARGN} ${_skip_dep})

    add_dependencies(deploy-all ${AT_TARGET})
endfunction(sm_add_tests)

# Use this function to diagnose link problems. Given a list of targets
# (most probably imported) it recurses through their interface link libraries
# (and their interface link libraries if they are targets and so on) and
# prints out the names of targets that have the specified DEPENDEE in their
# interface. The DEPENDEE must be a regex pattern. Using this function, you
# can investigate what imported targets cause linking of your target of
# interest to a certain library.
function (sm_diagnose_direct_dependencies DEPENDEE)
    macro (append_tgt_prop_to_list PROP)
        get_target_property(_prop_val ${_candidate} ${PROP})
        if (_prop_val)
            list(APPEND _libs ${_prop_val})
        endif (_prop_val)
    endmacro (append_tgt_prop_to_list)

    set(_candidate_dependencies ${ARGN})
    foreach (_candidate ${_candidate_dependencies})
        if (TARGET ${_candidate})
            set(_libs)
            append_tgt_prop_to_list(INTERFACE_LINK_LIBRARIES)
            get_target_property(_tgt_type ${_candidate} TYPE)
            if (NOT _tgt_type STREQUAL INTERFACE_LIBRARY)
                append_tgt_prop_to_list(IMPORTED_LOCATION)
                append_tgt_prop_to_list(LINK_INTERFACE_LIBRARIES)
                append_tgt_prop_to_list(IMPORTED_LINK_INTERFACE_LIBRARIES)
                append_tgt_prop_to_list(LINK_LIBRARIES)
            endif (NOT _tgt_type STREQUAL INTERFACE_LIBRARY)
            set(_test_iface_libs ${_libs})
            if (_test_iface_libs)
                list(FILTER _test_iface_libs INCLUDE REGEX ${DEPENDEE})
            endif (_test_iface_libs)
            if (_test_iface_libs)
                message("Target ${_candidate} interface links to ${DEPENDEE}")
                message("   Its direct dependencies are:")
                foreach (_direct_dep ${_libs})
                    message("       - ${_direct_dep}")
                endforeach (_direct_dep ${_libs})
            endif (_test_iface_libs)
            sm_diagnose_direct_dependencies(
                _tgt_dependencies ${DEPENDEE} ${_libs})
        endif (TARGET ${_candidate})
    endforeach (_candidate)
endfunction (sm_diagnose_direct_dependencies)

# Use this function to manipulate a target property the same as you manipulate
# a list using the CMake list command.
# Sample usage:
# sm_tgt_prop_list(MyTarget INTERFACE_LINK_LIBRARIES REMOVE_ITEM ITKVtkGlue)
# If the property were in a list variable prop_list the CMake list command
# would be used like this:
# list(REMOVE_ITEM prop_list ITKVtkGlue)
function (sm_tgt_prop_list _list_cmd _tgt _prop)
    if (NOT TARGET ${_tgt})
        return()
    endif (NOT TARGET ${_tgt})
    get_target_property(_prop_list ${_tgt} ${_prop})
    if (_prop_list)
        list(${_list_cmd} _prop_list ${ARGN})
        set_target_properties(${_tgt} PROPERTIES ${_prop} "${_prop_list}")
    endif (_prop_list)
endfunction (sm_tgt_prop_list)
