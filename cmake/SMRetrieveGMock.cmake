# This module creates a target to download, unpack, build, and install
# GTest and GMock (known together as GoogleTest Framework) using
# sm_add_ext_cmake_project and adds some build customization
# parameters. See sm_add_ext_cmake_project for more info.
if(SM_RETRIEVE_GMOCK_INCLUDED)
    return()
endif(SM_RETRIEVE_GMOCK_INCLUDED)
set(SM_RETRIEVE_GMOCK_INCLUDED 1)

include(SMUtil)

# The MSVS 2017 (19.10+) can't build GTest v1.8.0, so we'll have to upgrade to
# at least v1.8.1 (laterst at the time). But GTest v1.8.1, for some reason
# produces links errors on other toolsets (at least on Xcode 9.4) when
# building tests! So for toolsets other than MSVCv141 and above we'll keep
# using v1.8.0 until a new version is released.
if (MSVC AND MSVC_VERSION GREATER_EQUAL 1910)
    set(GTEST_URL https://github.com/google/googletest/archive/release-1.8.1.tar.gz)
    set(GTEST_SHA1 152b849610d91a9dfa1401293f43230c2e0c33f8)
else (MSVC AND MSVC_VERSION GREATER_EQUAL 1910)
    set(GTEST_URL https://github.com/google/googletest/archive/release-1.8.0.tar.gz)
    set(GTEST_SHA1 e7e646a6204638fe8e87e165292b8dd9cd4c36ed)
endif (MSVC AND MSVC_VERSION GREATER_EQUAL 1910)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
sm_add_ext_cmake_project(
    GMock
    URL ${GTEST_URL}
    SHA1 ${GTEST_SHA1}
    PDB_INSTALL_DIR lib
    EXTRA_CACHE_ARGS
        -DCMAKE_DEBUG_POSTFIX:STRING=d
        # Our projects use shared CRT, so other libraries that we link to must
        # be the same or we get link errors.
        -Dgtest_force_shared_crt:BOOL=ON
)
