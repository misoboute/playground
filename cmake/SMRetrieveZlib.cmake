# This module creates a target to download, unpack, build, and install
# ZLIB using sm_add_ext_cmake_project. See sm_add_ext_cmake_project
# for more info.
if(SM_RETRIEVE_ZLIB_INCLUDED)
    return()
endif(SM_RETRIEVE_ZLIB_INCLUDED)
set(SM_RETRIEVE_ZLIB_INCLUDED 1)

include(SwissMedaUtil)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
sm_add_ext_cmake_project(
    ZLIB
    URL https://www.zlib.net/zlib-1.2.11.tar.gz
    SHA1 e6d119755acdf9104d7ba236b1242696940ed6dd
    PDB_INSTALL_DIR lib
    EXTRA_CACHE_ARGS
        -DCMAKE_DEBUG_POSTFIX:STRING=d
)
