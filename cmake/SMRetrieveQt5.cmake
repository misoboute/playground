# This module creates targets for retrieving and extracting of the Qt
# Framework 5.
if(SM_RETRIEVE_QT5_INCLUDED)
    return()
endif(SM_RETRIEVE_QT5_INCLUDED)
set(SM_RETRIEVE_QT5_INCLUDED 1)

include(SwissMedaUtil)

if (WIN32)
    set(QT_PROJ_NAME Qt5_12_1)
    set(QT_DL_URL
        https://swissmeda-dev.s3.amazonaws.com/Qt5.12.1_vs2017.tar.bz2)
    set(QT_DL_SHA1 fdc458f196fa0a69c6dca1e043810d9da34160f9)
    set(QT_INSTALL_DIR ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/Qt5.12.1/msvc2017_64)
elseif (APPLE)
    set(QT_PROJ_NAME Qt5_12_1)
    set(QT_DL_URL https://swissmeda-dev.s3.amazonaws.com/Qt5.12.1_mac.tar.bz2)
    set(QT_DL_SHA1 c8cce605e5e033c2e483203e354cb2e8fe092af8)
    set(QT_INSTALL_DIR ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/Qt5.12.1)
endif (WIN32)

file(REMOVE_RECURSE
    ${SWISSMEDA_3RDPARTY_BUILD_DIR}/Stamp/${QT_PROJ_NAME}
    ${SWISSMEDA_3RDPARTY_BUILD_DIR}/tmp/${QT_PROJ_NAME}
)

ExternalProject_Add(
    ${QT_PROJ_NAME}
    URL ${QT_DL_URL}
    URL_HASH SHA1=${QT_DL_SHA1}
    DOWNLOAD_DIR ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Download/${QT_PROJ_NAME}
    SOURCE_DIR ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Source/${QT_PROJ_NAME}
    INSTALL_DIR ${QT_INSTALL_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND
        ${CMAKE_COMMAND} -E copy_directory <SOURCE_DIR> <INSTALL_DIR>
)
