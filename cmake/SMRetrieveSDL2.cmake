# This module creates a target to download, unpack, build, and install
# Simple Directmedia Layer 2 (SDL2) using sm_add_ext_cmake_project.
# See sm_add_ext_cmake_project for more info.
if(SM_RETRIEVE_SDL_INCLUDED)
    return()
endif(SM_RETRIEVE_SDL_INCLUDED)
set(SM_RETRIEVE_SDL_INCLUDED 1)

include(SwissMedaUtil)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
sm_add_ext_cmake_project(
    SDL2
    URL https://libsdl.org/release/SDL2-2.0.8.tar.gz
    SHA1 959ec6ded277821995d2e3cafd973f6ab9f48ab7
    PDB_INSTALL_DIR lib
    EXTRA_CACHE_ARGS
        -DSDL_STATIC:BOOL=ON
        -DSDL_SHARED:BOOL=OFF
        -DCMAKE_DEBUG_POSTFIX:STRING=d
)
