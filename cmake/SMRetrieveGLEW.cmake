# This module creates a target to download, unpack, build, and install
# GLEW using sm_add_ext_cmake_project. See sm_add_ext_cmake_project
# for more info.
if(SM_RETRIEVE_GLEW_INCLUDED)
	return()
endif(SM_RETRIEVE_GLEW_INCLUDED)
set(SM_RETRIEVE_GLEW_INCLUDED 1)

include(SwissMedaUtil)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
sm_add_ext_cmake_project(
	GLEW
	URL https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz
	SHA1 18eca05460d0b61709fc115e632b6fe320718b44
	SOURCE_SUBDIR build/cmake
	EXTRA_CACHE_ARGS
		-DCMAKE_FDEBUG_POSTFIX:STRING=d
)
