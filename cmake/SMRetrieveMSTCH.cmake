if(SM_RETRIEVE_MSTCH_INCLUDED)
	return()
endif(SM_RETRIEVE_MSTCH_INCLUDED)
set(SM_RETRIEVE_MSTCH_INCLUDED 1)

include(SwissMedaUtil)

set(Boost_USE_STATIC_LIBS true)
sm_find_or_get(Boost REQUIRED)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.

sm_add_ext_cmake_project(
	MSTCH
	URL https://github.com/no1msd/mstch/archive/1.0.2.tar.gz
	SHA1 e847bb682e1f97149a5f8284deb1038d15776236
    EXTRA_CACHE_ARGS
        -DCMAKE_DEBUG_POSTFIX:STRING=d
)