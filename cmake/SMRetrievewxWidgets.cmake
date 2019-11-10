# This module creates a target to download, unpack, build, and install
# wxWidgets using sm_add_ext_cmake_project. See sm_add_ext_cmake_project
# for more info.
if(SM_RETRIEVE_WXWIDGETS_INCLUDED)
    return()
endif(SM_RETRIEVE_WXWIDGETS_INCLUDED)
set(SM_RETRIEVE_WXWIDGETS_INCLUDED 1)

include(SMUtil)

sm_find_or_get(TIFF)
sm_find_or_get(ZLIB)

set(WX_SOURCE_PKG_URL
    https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.1/wxWidgets-3.1.1.7z)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
set(WX_SOURCE_PKG_SHA1 8d98975eb9f81036261c0643755b98e4bb5ab776)

set(WX_SM_VENDOR misoboute)

# The latest stable release of wxWidgets at this time is 3.0.4 which lacks
# a CMake generated build system. Building wxWidgets using CMake comes as a new
# feature starting with wxWidgets 3.1.1 which is the latest development release.
# Building with CMake being a new feature, it has apparently not matured yet
# and is problematic on Unix. On the other hand, build using CMake is a godsend
# on Windows! Without it, we'd have to install python, perl, and MSYS... and
# use the traditional build that is customary of Unix OS's. Therefore, until
# a stable release of wxWidgets with CMake building comes out (wxWidgets 3.2 ?)
# we have to use CMake on Windows and the configure scripts on OSx.

if(WIN32)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(WX_LIB_INSTALL_DIR lib/vc_x64_lib)
    else(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(WX_LIB_INSTALL_DIR lib/vc_lib)
    endif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    sm_add_ext_cmake_project(
        wxWidgets
        URL ${WX_SOURCE_PKG_URL}
        SHA1 ${WX_SOURCE_PKG_SHA1}
        PDB_INSTALL_DIR ${WX_LIB_INSTALL_DIR}
        EXTRA_CACHE_ARGS
            -DwxBUILD_SHARED:BOOL=OFF
            "-DwxBUILD_VENDOR:STRING=${WX_SM_VENDOR}"
            -DwxUSE_EXPAT:STRING=builtin
            -DwxUSE_LIBJPEG:STRING=builtin
            -DwxUSE_LIBPNG:STRING=builtin
            -DwxUSE_LIBTIFF:STRING=sys
            -DwxUSE_ZLIB:STRING=sys
    )

elseif(APPLE)
    sm_clean_ext_proj_build_sys_files(
        ${SM_3RDPARTY_BUILD_DIR}/wxwidgets wxwidgets)

    ExternalProject_Add(
        wxwidgets
        URL ${WX_SOURCE_PKG_URL}
        URL_HASH SHA1=${WX_SOURCE_PKG_SHA1}
        DOWNLOAD_DIR ${SM_DOWNLOAD_CACHE_DIR}/Download/wxwidgets
        SOURCE_DIR ${SM_DOWNLOAD_CACHE_DIR}/Source/wxwidgets
        BINARY_DIR ${SM_3RDPARTY_BUILD_DIR}/wxwidgets
        INSTALL_DIR ${SM_3RDPARTY_INSTALL_DIR}
        CONFIGURE_COMMAND
            <SOURCE_DIR>/configure
                --prefix=<INSTALL_DIR> --disable-shared
                --enable-vendor="${WX_SM_VENDOR}"
                --with-macosx-version-min=${CMAKE_OSX_DEPLOYMENT_TARGET}
                --enable-debug --enable-debug_info --without-subdirs
                --with-libjpeg --with-libpng --with-libtiff --with-zlib
                --with-expat --enable-xrc
    )

    sm_clean_ext_proj_build_sys_files(${SM_3RDPARTY_BUILD_DIR}/wxrc wxrc)
    ExternalProject_Add(
        wxrc
        SOURCE_DIR ${SM_DOWNLOAD_CACHE_DIR}/Source/wxwidgets/utils/wxrc
        BINARY_DIR ${SM_3RDPARTY_BUILD_DIR}/wxwidgets/utils/wxrc
        INSTALL_DIR ${SM_3RDPARTY_INSTALL_DIR}
        DOWNLOAD_COMMAND ""
        CMAKE_CACHE_DEFAULT_ARGS
            -DCMAKE_INSTALL_PREFIX:STRING=<INSTALL_DIR>
            -DCMAKE_PREFIX_PATH:STRING=${CMAKE_PREFIX_PATH}
            -DCMAKE_OSX_DEPLOYMENT_TARGET:STRING=${CMAKE_OSX_DEPLOYMENT_TARGET}
    )
    sm_add_ext_proj_patch_step(wxrc wxrc)

    add_dependencies(wxrc wxwidgets)
endif(WIN32)
