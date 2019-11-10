# This module creates a target to download, unpack, build, and install
# freeCGI v2.4.11 using sm_add_ext_cmake_project.
# See sm_add_ext_cmake_project for more info.
if(SM_RETRIEVE_FREECGI_INCLUDED)
    return()
endif(SM_RETRIEVE_FREECGI_INCLUDED)
set(SM_RETRIEVE_FREECGI_INCLUDED 1)

include(SwissMedaUtil)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
sm_add_ext_cmake_project(
    freeCGI
    URL https://github.com/achacha/freeCGI/archive/84c047c4790eaabbc9d2284242a44c204fe674ea.zip
    SHA1 904c2cd7d629a205469c5e2db5b7d5b27785adb1
    EXTRA_CACHE_ARGS
        -DCMAKE_DEBUG_POSTFIX:STRING=d
)
