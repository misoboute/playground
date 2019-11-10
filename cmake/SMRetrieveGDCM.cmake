# This module creates a target to download, unpack, build, and install
# GDCM 2 using sm_add_ext_cmake_project. See sm_add_ext_cmake_project
# for more info.
if(SM_RETRIEVE_GDCM_INCLUDED)
    return()
endif(SM_RETRIEVE_GDCM_INCLUDED)
set(SM_RETRIEVE_GDCM_INCLUDED 1)

include(SwissMedaUtil)

# Make sure ZLIB and VTK are present.
set(ZLIB_FIND_STATIC ON)
sm_find_or_get(ZLIB REQUIRED)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
sm_add_ext_cmake_project(
    GDCM
    URL https://github.com/malaterre/GDCM/archive/v2.8.7.zip
    SHA1 2b97f268cbc9e6273d1ede376c552c69f48a35a4 
    PDB_INSTALL_DIR lib
    EXTRA_CACHE_ARGS
        -DCMAKE_DEBUG_POSTFIX:STRING=d
        -DGDCM_USE_SYSTEM_ZLIB:BOOL=ON
        # We use the ZLIB found by our own module (statically linked)
        "-DZLIB_LIBRARY:STRING=${ZLIB_LIBRARIES}"
        # GDCM v2.8.7 doesn't work with VTK 8+
        -DGDCM_USE_VTK:BOOL=OFF
        -DGDCM_BUILD_TESTING:BOOL=OFF
        -DGDCM_BUILD_EXAMPLES:BOOL=OFF
        -DGDCM_BUILD_DOCBOOK_MANPAGES:BOOL=OFF
        -DOPENJPEG_NAMESPACE:STRING=IPP_GDCM_OPENJPEG_NAMESPACE
    EXTRA_CACHE_ARGS_APPLE
        -DOPENJPEG_LIBRARY_NAME:STRING=ipp_gdcm_openjpeg
    EXTRA_CACHE_ARGS_WIN32
        # GDCM has an install rule meant for CPack that installs VTK DLL's
        # (apart from GDCM's own libs) when GDCM_USE_VTK. But since we build
        # VTK without shared libs, we don't have those DLL's. And since we are
        # not CPack'ing GDCM anyway, we don't need that install rule and can
        # skip it using this [undocumented] flag:
        -DCMAKE_INSTALL_VTK_RUNTIME_LIBS_SKIP:BOOL=ON
)
