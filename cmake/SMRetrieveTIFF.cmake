# This module creates a target to download, unpack, build, and install
# TIFF using sm_add_ext_cmake_project and adds some build customization
# parameters. See sm_add_ext_cmake_project for more info.
if(SM_RETRIEVE_TIFF_INCLUDED)
    return()
endif(SM_RETRIEVE_TIFF_INCLUDED)
set(SM_RETRIEVE_TIFF_INCLUDED 1)

include(SMUtil)

sm_find_or_get(ZLIB)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
sm_add_ext_cmake_project(
    TIFF
    URL https://gitlab.com/libtiff/libtiff/repository/Release-v4-0-9/archive.zip
    SHA1 c631e6daeb3054c3429781952fe17ceecc09af66
    PDB_INSTALL_DIR lib
    EXTRA_CACHE_ARGS
        -DCMAKE_DEBUG_POSTFIX:STRING=d
        # We use the ZLIB found by our own module (statically linked)
        "-DZLIB_LIBRARY:STRING=${ZLIB_LIBRARIES}"
        -Dlzma:BOOL=OFF
        -Djpeg:BOOL=OFF
        -Dold-jpeg:BOOL=OFF
)
