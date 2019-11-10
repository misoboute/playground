# This module creates targets for retrieving and extracting (or building)
# of GNU gettext
if(SM_RETRIEVE_GETTEXT_INCLUDED)
    return()
endif(SM_RETRIEVE_GETTEXT_INCLUDED)
set(SM_RETRIEVE_GETTEXT_INCLUDED 1)

include(SwissMedaUtil)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
if (WIN32)
    if (SM_IS_64_BIT)
        set(GETTEXT_DL_URL
            https://github.com/mlocati/gettext-iconv-windows/releases/download/v0.19.8.1-v1.15/gettext0.19.8.1-iconv1.15-shared-64.zip)
        set(GETTEXT_DL_SHA1 ee84d14065ff518a57b8bd517627fb71c886b031)
    else (SM_IS_64_BIT)
        set(GETTEXT_DL_URL
            https://github.com/mlocati/gettext-iconv-windows/releases/download/v0.19.8.1-v1.15/gettext0.19.8.1-iconv1.15-shared-32.zip)
        set(GETTEXT_DL_SHA1 3ccc5c0da936fd09b4554cfd3767279e03f711f2)
    endif (SM_IS_64_BIT)
    ExternalProject_Add(
        gettext
        URL ${GETTEXT_DL_URL}
        URL_HASH SHA1=${GETTEXT_DL_SHA1}
        DOWNLOAD_DIR
            ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Download/gettext
        SOURCE_DIR ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Source/gettext
        INSTALL_DIR ${SWISSMEDA_3RDPARTY_INSTALL_DIR}
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND
            ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR> <INSTALL_DIR>
    )
else (WIN32)
    sm_clean_ext_proj_build_sys_files(
        ${SWISSMEDA_3RDPARTY_BUILD_DIR}/gettext gettext)

    ExternalProject_Add(
        gettext
        URL http://ftp.gnu.org/pub/gnu/gettext/gettext-0.20.tar.gz
        URL_HASH SHA1=58AD0E554D5DFB537FF0FF1E604220F327DE2889
        DOWNLOAD_DIR
            ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Download/gettext
        SOURCE_DIR ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Source/gettext
        BINARY_DIR ${SWISSMEDA_3RDPARTY_BUILD_DIR}/gettext
        INSTALL_DIR ${SWISSMEDA_3RDPARTY_INSTALL_DIR}
        CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
        BUILD_COMMAND make
        INSTALL_COMMAND make install
    )
endif (WIN32)
# online md5 sha1  generator http://onlinemd5.com
