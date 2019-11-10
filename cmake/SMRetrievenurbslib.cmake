# This module creates a target to download, unpack, build, and install
# libnurbs using sm_add_ext_cmake_project and adds some build customization
# parameters. See sm_add_ext_cmake_project for more info.
if(SM_RETRIEVE_LIBNURBS_INCLUDED)
    return()
endif(SM_RETRIEVE_LIBNURBS_INCLUDED)
set(SM_RETRIEVE_LIBNURBS_INCLUDED 1)

include(SwissMedaUtil)

# Make sure ZLIB and openNURBS are present.
sm_find_or_get(ZLIB)
sm_find_or_get(openNURBS)

if (NOT EXISTS ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/pbourke)
    # We're probably in a subdirectory of the original 3rd party install dir
    # which we use for the unified smopapp:
    get_filename_component(ORIG_3RDPARTY_ROOT
        ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/.. ABSOLUTE)
else (NOT EXISTS ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/pbourke)
    set(ORIG_3RDPARTY_ROOT ${SWISSMEDA_3RDPARTY_INSTALL_DIR})
endif (NOT EXISTS ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/pbourke)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
#     SHA1 5bf626f32b0e3389da835ee4729ea36b0b4c2f95 before 2018Aug
#     7dc870c3721e76f83a838fcfd3bb3d9df35e102d without earcut
#     58ff1d01247d9cd832c022020e7c70f03d69cf75 with earcut
#     d4548a13f21cbe040bb03c05abd64fe29cbffb97 with makeSolid2
#     3a74045012230015195efaad4301bd790de42287 MacOS scope fix
sm_add_ext_cmake_project(
    nurbslib
    URL  https://swissmeda-dev.s3.amazonaws.com/nurbslib-768e1aa05540670a61c419ab97921fc80411ea41.tar.bz2
    SHA1 768e1aa05540670a61c419ab97921fc80411ea41
    PDB_INSTALL_DIR lib
    EXTRA_CACHE_ARGS
        -DCMAKE_DEBUG_POSTFIX:STRING=d
        -DCMAKE_MODULE_PATH:STRING=${CMAKE_MODULE_PATH}
)
