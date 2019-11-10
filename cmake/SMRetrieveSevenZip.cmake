# This module creates targets for retrieving and extracting of the Qt
# Framework 5.
if(SM_RETRIEVE_SEVEN_ZIP_INCLUDED)
    return()
endif(SM_RETRIEVE_SEVEN_ZIP_INCLUDED)
set(SM_RETRIEVE_SEVEN_ZIP_INCLUDED 1)

if (NOT WIN32)
    message(AUTHOR_WARNING "7-Zip binaries are only available for Windows")
    return()
endif (NOT WIN32)

include(SwissMedaUtil)

if (SM_IS_64_BIT)
    set(SEVEN_ZIP_DL_URL
        https://swissmeda-dev.s3.amazonaws.com/7-Zip-18.05-win64.zip)
    set(SEVEN_ZIP_DL_SHA1 6ac243585b80d315b5f08e0d9a0d17bc745a34b9)
else (SM_IS_64_BIT)
    set(SEVEN_ZIP_DL_URL
        https://swissmeda-dev.s3.amazonaws.com/7-Zip-18.05-win32.zip)
    set(SEVEN_ZIP_DL_SHA1 fa565d900809e79f77093ee78b9053d5edcc6773)
endif (SM_IS_64_BIT)

set(_proj_name SevenZip)

ExternalProject_Add(
    ${_proj_name}
    URL ${SEVEN_ZIP_DL_URL}
    URL_HASH SHA1=${SEVEN_ZIP_DL_SHA1}
    DOWNLOAD_DIR ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Download/${_proj_name}
    SOURCE_DIR ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Source/${_proj_name}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E
        copy_directory <SOURCE_DIR> ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/7-Zip
)
