# This module creates a target to download, unpack, build, and install
# JSON Spirit using sm_add_ext_cmake_project and some customization depending whether
# the platform is Apple or Windows. See sm_add_ext_cmake_project
# for more info.
if(SM_RETRIEVE_JSON_SPIRIT_INCLUDED)
    return()
endif(SM_RETRIEVE_JSON_SPIRIT_INCLUDED)
set(SM_RETRIEVE_JSON_SPIRIT_INCLUDED 1)

include(SwissMedaUtil)

# Make sure Boost is present.
sm_find_or_get(Boost)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
sm_add_ext_cmake_project(
    JSONSpirit
    URL https://github.com/png85/json_spirit/archive/json_spirit-4.0.8.zip
    SHA1 d085b15a935743ead38fd36e3fc1081a0d3e5ef8
    EXTRA_CACHE_ARGS
        -DBUILD_STATIC_LIBRARIES:BOOL=OFF
        -DBUILD_STATIC_LIBS:BOOL=OFF
        -DCMAKE_DEBUG_POSTFIX:STRING=d
        -DJSON_SPIRIT_WVALUE_ENABLED:BOOL=ON
        -DJSON_SPIRIT_MVALUE_ENABLED:BOOL=OFF
        -DJSON_SPIRIT_WMVALUE_ENABLED:BOOL=OFF
        -DJSON_SPIRIT_DEMOS:BOOL=OFF
        -DJSON_SPIRIT_TESTS:BOOL=OFF
)
