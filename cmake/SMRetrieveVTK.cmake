# This module creates a target to download, unpack, build, and install
# VTK 7 or 8 (depending on which one is requested in SM_VTK_REQUESTED_VERSION)
# using sm_add_ext_cmake_project and some customization
# depending whether the platform is Apple or Windows.
# See sm_add_ext_cmake_project for more info.
if(SM_RETRIEVE_VTK_INCLUDED)
    return()
endif(SM_RETRIEVE_VTK_INCLUDED)
set(SM_RETRIEVE_VTK_INCLUDED 1)

include(SwissMedaUtil)

# Make sure ZLIB, TIFF, and Qt5Core are present.
set(ZLIB_FIND_STATIC ON)
sm_find_or_get(ZLIB)
sm_find_or_get(TIFF)
sm_find_or_get(Qt5 COMPONENTS Core VERIF_TARGET Qt5::Core)

if (SM_VTK_REQUESTED_VERSION VERSION_GREATER_EQUAL 8)
    # The URL HASH created using online facility:
    # 	https://hash.online-convert.com/sha1-generator
    sm_add_ext_cmake_project(
        VTK8
        URL https://github.com/Kitware/VTK/archive/v8.1.1.tar.gz
        SHA1 c517402144bffa2ae8a0c7537034a10e6f9e7e7c
        PDB_INSTALL_DIR lib
        EXTRA_CACHE_ARGS
            -DCMAKE_DEBUG_POSTFIX:STRING=d
            -DVTK_USE_SYSTEM_TIFF:BOOL=ON
            -DVTK_USE_SYSTEM_ZLIB:BOOL=ON
            # We use the ZLIB found by our own module (statically linked)
            "-DZLIB_LIBRARY:STRING=${ZLIB_LIBRARIES}"
            -DVTK_QT_VERSION:STRING=5 
            -DVTK_Group_Qt:BOOL=ON 
            -DVTK_BUILD_QT_DESIGNER_PLUGIN:BOOL=OFF
            ${_set_vtk_extra_prefix}
        EXTRA_CACHE_ARGS_WIN32
            -DCMAKE_CXX_MP_FLAG:BOOL=ON
    )
    return()
endif (SM_VTK_REQUESTED_VERSION VERSION_GREATER_EQUAL 8)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
sm_add_ext_cmake_project(
    VTK7
    URL https://gitlab.kitware.com/vtk/vtk/repository/6f3b9af1ecc8987ec55732a2bef2b5b544fd1526/archive.tar.bz2
    SHA1 17bdf554100c5f9f5faf487a2740b7e75448544a
    PDB_INSTALL_DIR lib
    EXTRA_CACHE_ARGS
        -DCMAKE_DEBUG_POSTFIX:STRING=d
        -DVTK_USE_SYSTEM_ZLIB:BOOL=ON
        # We use the ZLIB found by our own module (statically linked)
        -DZLIB_LIBRARY:STRING=${ZLIB_LIBRARIES}
        -DVTK_USE_SYSTEM_TIFF:BOOL=ON
        -DVTK_QT_VERSION:STRING=5 
        -DVTK_Group_Qt:BOOL=ON 
        -DVTK_Group_Rendering:BOOL=ON 
        -DVTK_Group_StandAlone:BOOL=ON
        -DVTK_BUILD_QT_DESIGNER_PLUGIN:BOOL=OFF
        -DVTK_USE_64BIT_IDS:BOOL=ON
    EXTRA_CACHE_ARGS_WIN32
        -DCMAKE_CXX_MP_FLAG:BOOL=ON
)
