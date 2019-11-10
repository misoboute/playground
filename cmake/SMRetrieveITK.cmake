# This module creates a target to download, unpack, build, and install
# ITK using sm_add_ext_cmake_project. See sm_add_ext_cmake_project
# for more info.
if(SM_RETRIEVE_ITK_INCLUDED)
    return()
endif(SM_RETRIEVE_ITK_INCLUDED)
set(SM_RETRIEVE_ITK_INCLUDED 1)

include(SwissMedaUtil)

# Make sure ZLIB and GDCM are present.
set(ZLIB_FIND_STATIC ON)
sm_find_or_get(ZLIB REQUIRED)
sm_find_or_get(GDCM)
sm_find_or_get(VTK REQUIRED)

if (SM_EXTRA_PREFIX)
    sm_find_or_get(VTK EXTRA_PREFIX ${SM_EXTRA_PREFIX} REQUIRED)
else (SM_EXTRA_PREFIX)
    sm_find_or_get(VTK REQUIRED)
endif (SM_EXTRA_PREFIX)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
sm_add_ext_cmake_project(
    ITK4_12
    URL https://github.com/InsightSoftwareConsortium/ITK/releases/download/v4.12.2/InsightToolkit-4.12.2.zip
    SHA1 52e5145b22a7522f11c12cff063aa91cb6eb7c1b
    PDB_INSTALL_DIR lib
    EXTRA_CACHE_ARGS
        -DCMAKE_DEBUG_POSTFIX:STRING=d
        -DITK_USE_SYSTEM_ZLIB:BOOL=ON
        # We use the ZLIB found by our own module (statically linked)
        -DZLIB_LIBRARY:STRING=${ZLIB_LIBRARIES}
        -DITK_USE_SYSTEM_TIFF:BOOL=ON
        -DITK_USE_SYSTEM_GDCM:BOOL=ON
        -DModule_ITKReview:BOOL=ON
        -DModule_ITKVtkGlue:BOOL=ON # for itkVTKImageToImageFilter
        # Use the same VTK as we have found here using sm_find_or_get
        -DVTK_ROOT:PATH=${VTK_DIR}
    EXTRA_CACHE_ARGS_WIN32
        -DITK_REQUIRED_CXX_FLAGS:STRING=/MP
)
