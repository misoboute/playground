set(_current_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
if(WIN32)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dll")
else(WIN32)
    SET(CMAKE_FIND_LIBRARY_SUFFIXES ".dylib")
endif(WIN32)

find_library(QtCipherSqlitePlugin_LIBRARY_RELEASE
    NAMES sqlitecipher
    PATH_SUFFIXES plugins/sqldrivers
)
find_library(QtCipherSqlitePlugin_LIBRARY_DEBUG
    NAMES sqlitecipherd
    PATH_SUFFIXES plugins/sqldrivers
)
set(CMAKE_FIND_LIBRARY_SUFFIXES ${_current_CMAKE_FIND_LIBRARY_SUFFIXES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QtCipherSqlitePlugin
    DEFAULT_MSG
    QtCipherSqlitePlugin_LIBRARY_RELEASE
)

if(QtCipherSqlitePlugin_FOUND)
    if(QtCipherSqlitePlugin_LIBRARY_DEBUG)
        set(QtCipherSqlitePlugin_LIBRARIES
            optimized ${QtCipherSqlitePlugin_LIBRARY_RELEASE}
            debug ${QtCipherSqlitePlugin_LIBRARY_DEBUG}
        )
    else(QtCipherSqlitePlugin_LIBRARY_DEBUG)
        set(QtCipherSqlitePlugin_LIBRARIES
            ${QtCipherSqlitePlugin_LIBRARY_RELEASE})
    endif(QtCipherSqlitePlugin_LIBRARY_DEBUG)
    # sm_find_or_get expects an include directory variable to check whether
    # the include directories are inside the swissmeda 3rd party install
    # directory.
    get_filename_component(
        QtCipherSqlitePlugin_INCLUDE_DIRS
        ${QtCipherSqlitePlugin_LIBRARY_RELEASE}/../../include ABSOLUTE
    )
endif(QtCipherSqlitePlugin_FOUND)

mark_as_advanced(
    QtCipherSqlitePlugin_LIBRARY_RELEASE
    QtCipherSqlitePlugin_LIBRARY_DEBUG
    QtCipherSqlitePlugin_LIBRARIES
)
