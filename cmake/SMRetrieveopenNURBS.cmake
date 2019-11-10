# This module creates a target to download, unpack, build, and install
# openNURBS using sm_add_ext_cmake_project and adds some build customization
# parameters. See sm_add_ext_cmake_project for more info.
if(SM_RETRIEVE_OPENNURBS_INCLUDED)
    return()
endif(SM_RETRIEVE_OPENNURBS_INCLUDED)
set(SM_RETRIEVE_OPENNURBS_INCLUDED 1)

include(SMUtil)

# Make sure ZLIB is present.
sm_find_or_get(ZLIB)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
sm_add_ext_cmake_project(
    openNURBS
    URL https://github.com/louipc/opennurbs/archive/5.0.20130711.tar.gz
    SHA1 323c0b2b044341de9251c9994834f94a3bc7684d
    EXTRA_CACHE_ARGS
        -DCMAKE_DEBUG_POSTFIX:STRING=d
)
