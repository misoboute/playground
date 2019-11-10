# This module creates targets for retrieving, building, and installing of the
# Boost C++ libraries.
if(SM_RETRIEVE_BOOST_INCLUDED)
    return()
endif(SM_RETRIEVE_BOOST_INCLUDED)
set(SM_RETRIEVE_BOOST_INCLUDED 1)

include(SwissMedaUtil)

if (${CMAKE_GENERATOR} MATCHES "Visual Studio 15 2017")
    set(BOOST_BUILD_TOOLSET msvc-14.1)
    set(BOOST_DL_URL
        https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.7z)
    set(BOOST_DL_SHA1 64c278c23defe155e630a307ae2c0615348b14b3)
elseif (${CMAKE_GENERATOR} MATCHES "Visual Studio 14 2015")
    set(BOOST_BUILD_TOOLSET msvc-14.0)
    set(BOOST_DL_URL
        https://dl.bintray.com/boostorg/release/1.67.0/source/boost_1_67_0.7z)
    set(BOOST_DL_SHA1 64c278c23defe155e630a307ae2c0615348b14b3)
elseif (APPLE)
    set(BOOST_BUILD_TOOLSET darwin)
    set(BOOST_DL_URL
        https://dl.bintray.com/boostorg/release/1.71.0/source/boost_1_71_0.tar.bz2)
    set(BOOST_DL_SHA1 d73a8da01e8bf8c7eda40b4c84915071a8c8a0df4a6734537ddde4a8580524ee)
endif (${CMAKE_GENERATOR} MATCHES "Visual Studio 15 2017")
message("BOOST_BUILD_TOOLSET: ${BOOST_BUILD_TOOLSET}")

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(_boost_address_model 64)
else(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(_boost_address_model 32)
endif(CMAKE_SIZEOF_VOID_P EQUAL 8)

set(_boost_src_dir ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Source/boost)
if(WIN32)
    # The bootstrap must be run from an MSVS developer command prompt.
    # We shall set the environment instead using vcvarsall in a batch
    # file.
    # VS 2017 vcvarsall changes directory to a different path and the build
    # is messed up. Must CD to the sources directory after vcvarsall.
    # https://stackoverflow.com/questions/46681881/visual-studio-2017-developer-command-prompt-switches-current-directory?rq=1
    sm_find_vcvarsall(_vcvarsall STRING REQUIRED)
    file(TO_NATIVE_PATH ${_boost_src_dir} _boost_src_dir_native)
    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/boost_boostrap.bat
"SET VSCMD_START_DIR=${_boost_src_dir_native}\\boost\\tools\\build\\src\\engine
${_vcvarsall}
CD \"${_boost_src_dir_native}\"
bootstrap.bat"
    )
    set(_boost_bootstrap_command
        ${CMAKE_CURRENT_BINARY_DIR}/boost_boostrap.bat)
    set(_boost_b2_command b2.exe)
elseif(APPLE)
    set(_boost_bootstrap_command ./bootstrap.sh)
    set(_boost_b2_command ./b2)
endif(WIN32)
message("_boost_bootstrap_command: ${_boost_bootstrap_command}")

sm_clean_ext_proj_build_sys_files(${SWISSMEDA_3RDPARTY_BUILD_DIR}/boost boost)

ExternalProject_Add(
    boost
    URL ${BOOST_DL_URL}
    URL_HASH SHA1=${BOOST_DL_SHA1}
    DOWNLOAD_DIR ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Download/boost
    SOURCE_DIR ${_boost_src_dir}
    BINARY_DIR ${SWISSMEDA_3RDPARTY_BUILD_DIR}/boost
    INSTALL_DIR ${SWISSMEDA_3RDPARTY_INSTALL_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
)

ExternalProject_Add_Step(
    boost bootstrap
    COMMENT "Configuring Boost using bootstrap..."
    WORKING_DIRECTORY <SOURCE_DIR>
    DEPENDEES patch
    DEPENDERS configure
    COMMAND ${_boost_bootstrap_command}
)

 # By default ZLIB use is disabled for Windows and on Unix enabled if
 # a ZLIB installation is found. We'll enable them to unify across all OS's.
 # See http://www.boost.org/doc/libs/1_50_0/libs/iostreams/doc/installation.html
set(ZLIB_FIND_STATIC ON)
sm_find_or_get(ZLIB REQUIRED)
if(ZLIB_FOUND)
    set(_boost_zlib_params
        -sNO_ZLIB=0
        -sZLIB_BINARY=$<TARGET_PROPERTY:ZLIB::ZLIB,LOCATION>
        -sZLIB_INCLUDE=${ZLIB_INCLUDE_DIRS}
        -sZLIB_LIBPATH=$<TARGET_FILE_DIR:ZLIB::ZLIB>
    )
endif(ZLIB_FOUND)

# Complete command line options: b2.exe --help
# http://www.boost.org/build/doc/html/bbv2/overview/invocation.html
set (_boost_build_common_options
    -d1 # We won't need a lot debug output from the build
    -sBOOST_ROOT=<SOURCE_DIR>

    ${_boost_zlib_params}

    # Other layouts place the include directory inside a version suffixed directory
    # which can't be found by find_package. Since we use only one boost version,
    # we don't need 'versioned' or 'tagged' layouts.
    --layout=tagged
    --without-python
    --build-dir=<BINARY_DIR>
    --prefix=<INSTALL_DIR>
    --build-type=minimal
        # two configs: debug and release, multithread, shared runtime, static libs
    toolset=${BOOST_BUILD_TOOLSET}
    address-model=${_boost_address_model}   # 32/64
)

message("_boost_build_common_options: ${_boost_build_common_options}")

ExternalProject_Add_Step(
    boost b2_build
    COMMENT "Building Boost using b2..."
    WORKING_DIRECTORY <SOURCE_DIR>
    DEPENDEES configure
    DEPENDERS build
    COMMAND ${_boost_b2_command} -j8 ${_boost_build_common_options}
)

ExternalProject_Add_Step(
    boost b2_install
    COMMENT "Installing Boost using b2..."
    WORKING_DIRECTORY <SOURCE_DIR>
    DEPENDEES b2_build
    DEPENDERS install
    COMMAND ${_boost_b2_command} ${_boost_build_common_options} install
)
