# This module creates targets for retrieving, building, and installing of
# QtCipherSqlitePlugin.
if(SM_RETRIEVE_QT_CIPHER_SQLITE_PLUGIN_INCLUDED)
    return()
endif(SM_RETRIEVE_QT_CIPHER_SQLITE_PLUGIN_INCLUDED)
set(SM_RETRIEVE_QT_CIPHER_SQLITE_PLUGIN_INCLUDED 1)

include(SwissMedaUtil)

# Make sure that Qt is present
sm_find_or_get(Qt5 COMPONENTS Core VERIF_TARGET Qt5::Core)

sm_clean_ext_proj_build_sys_files(
    ${SWISSMEDA_3RDPARTY_BUILD_DIR}/sqlitecipher sqlitecipher)

find_program(QMAKE_EXEC qmake DOC "Complete path to the qmake executable")
if(NOT QMAKE_EXEC)
    message(FATAL
        "Failed to locate qmake executable. Can't build QtCipherSqlitePlugin")
    return()
endif(NOT QMAKE_EXEC)
get_filename_component(QT_BIN_DIR ${QMAKE_EXEC} DIRECTORY)
get_filename_component(QT_DIR ${QT_BIN_DIR}/.. ABSOLUTE)

execute_process(COMMAND ${QMAKE_EXEC} --version
    OUTPUT_VARIABLE _QT_VER_INFO OUTPUT_STRIP_TRAILING_WHITESPACE)
string(REGEX MATCH "Qt version (([0-9]+\\.)+[0-9]+)"
    _QT_VER_MATCH "${_QT_VER_INFO}")
set(QT_VERSION ${CMAKE_MATCH_1})

if (${CMAKE_GENERATOR} MATCHES "Visual Studio 15 2017")
    if (QT_VERSION VERSION_GREATER_EQUAL 5.11)
        set(QMAKESPEC win32-msvc)
    else (QT_VERSION VERSION_GREATER_EQUAL 5.11)
        set(QMAKESPEC win32-msvc2017)
    endif (QT_VERSION VERSION_GREATER_EQUAL 5.11)
elseif (${CMAKE_GENERATOR} MATCHES "Visual Studio 14 2015")
    set(QMAKESPEC win32-msvc2015)
elseif (${CMAKE_GENERATOR} MATCHES "Xcode")
    set(QMAKESPEC macx-xcode)
endif (${CMAKE_GENERATOR} MATCHES "Visual Studio 15 2017")

set(_src_dir ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Source/sqlitecipher)
set(_bin_dir ${SWISSMEDA_3RDPARTY_BUILD_DIR}/sqlitecipher)
file(TO_NATIVE_PATH ${QMAKE_EXEC} QMAKE_EXEC)
file(TO_NATIVE_PATH ${QT_BIN_DIR} QT_BIN_DIR)
file(TO_NATIVE_PATH ${QT_DIR} QT_DIR)
file(TO_NATIVE_PATH ${_bin_dir} BINARY_DIR)
file(TO_NATIVE_PATH ${_src_dir} SOURCE_DIR)
if(MSVC)
    set(_build_install_script ${CMAKE_CURRENT_BINARY_DIR}/build_install.bat)
    sm_find_vcvarsall(VCVARSALL_CMD STRING)
    configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/BuildInstallQtCipherSqlitePlugin.bat
        ${_build_install_script}
    )
else(MSVC)
    set(_build_install_script ${CMAKE_CURRENT_BINARY_DIR}/exec/build_install.sh)
    configure_file(
        ${CMAKE_CURRENT_LIST_DIR}/BuildInstallQtCipherSqlitePlugin.sh
        ${CMAKE_CURRENT_BINARY_DIR}/build_install.sh
    )
    file(COPY ${CMAKE_CURRENT_BINARY_DIR}/build_install.sh
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/exec
        FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ
            GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)
endif(MSVC)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
ExternalProject_Add(
    sqlitecipher
    URL https://github.com/devbean/QtCipherSqlitePlugin/archive/v1.2.tar.gz
    URL_HASH SHA1=f9425aa14598cd5f3d645e0b810d148252eadd1c
    DOWNLOAD_DIR ${SWISSMEDA_DOWNLOAD_CACHE_DIR}/Download/sqlitecipher
    SOURCE_DIR ${_src_dir}
    BINARY_DIR ${_bin_dir}
    INSTALL_DIR ${SWISSMEDA_3RDPARTY_INSTALL_DIR}
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ${_build_install_script}
)
