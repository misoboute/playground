# This module creates a target to download, unpack, build, and install
# libcurl using sm_add_ext_cmake_project.
# See sm_add_ext_cmake_project for more info.
if(SM_RETRIEVE_CURL_INCLUDED)
    return()
endif(SM_RETRIEVE_CURL_INCLUDED)
set(SM_RETRIEVE_CURL_INCLUDED 1)

include(SwissMedaUtil)

set(ZLIB_FIND_STATIC ON)
sm_find_or_get(ZLIB REQUIRED)
message("ZLIB for CURL: ${ZLIB_LIBRARIES}")

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
sm_add_ext_cmake_project(
    CURL
    URL https://curl.haxx.se/download/curl-7.59.0.tar.gz
    SHA1 1a9bd7e201e645207b23a4b4dc38a32cc494a638
    PDB_INSTALL_DIR lib
    EXTRA_CACHE_ARGS
        # We use the ZLIB found by our own module (statically linked)
        -DZLIB_LIBRARY:STRING=${ZLIB_LIBRARIES}
        -DCMAKE_DEBUG_POSTFIX:STRING=d
        -DCURL_STATICLIB:BOOL=ON
        -DENABLE_INET_PTON:BOOL=OFF
        -DCURL_CA_BUNDLE:STRING=none
        -DCURL_CA_PATH:STRING=none
    EXTRA_CACHE_ARGS_APPLE
        -DCMAKE_USE_DARWINSSL:BOOL=ON
    EXTRA_CACHE_ARGS_WIN32
        -DCMAKE_USE_WINSSL:BOOL=ON
)
