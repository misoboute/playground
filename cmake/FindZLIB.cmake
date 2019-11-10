# The original FindZLIB module shipped with the CMake can't find the static
# zlib libraries because the original module uses a different naming scheme
# than used by the ZLIB source. To find the static libraries, set the flag
# ZLIB_FIND_STATIC.
# The original also is happy if it finds either Debug or Release version of
# the library. But this might not be always desirable. For example, if we once
# did have both libraries, found them and stored them as dependencies, it will
# not be OK for one of them to be deleted afterwards. By setting the flag
# ZLIB_FIND_DEBUG_AND_RELEASE, signal that you need both Debug and Release
# versions of the library and the find should fail if both are not found.
set(_ZLIB_SEARCHES)

# Search ZLIB_ROOT first if it is set.
if(ZLIB_ROOT)
  set(_ZLIB_SEARCH_ROOT PATHS ${ZLIB_ROOT} NO_DEFAULT_PATH)
  list(APPEND _ZLIB_SEARCHES _ZLIB_SEARCH_ROOT)
endif()

# Normal search.
set(_ZLIB_SEARCH_NORMAL
  PATHS "[HKEY_LOCAL_MACHINE\\SOFTWARE\\GnuWin32\\Zlib;InstallPath]"
        "$ENV{PROGRAMFILES}/zlib"
  )
list(APPEND _ZLIB_SEARCHES _ZLIB_SEARCH_NORMAL)

if(ZLIB_FIND_STATIC)
    set(ZLIB_NAMES libz.a zlibstatic)
    set(ZLIB_NAMES_DEBUG libzd.a zlibstaticd)
else(ZLIB_FIND_STATIC)
    set(ZLIB_NAMES z zlib zdll zlib1)
    set(ZLIB_NAMES_DEBUG zlibd zlibd1)
endif(ZLIB_FIND_STATIC)

# Try each search configuration.
foreach(search ${_ZLIB_SEARCHES})
  find_path(ZLIB_INCLUDE_DIR NAMES zlib.h ${${search}} PATH_SUFFIXES include)
endforeach()

# Allow ZLIB_LIBRARY to be set manually, as the location of the zlib library
if(ZLIB_LIBRARY)
    if (ZLIB_FIND_DEBUG_AND_RELEASE)
        message("Setting ZLIB_LIBRARY precedes ZLIB_FIND_DEBUG_AND_RELEASE.
Turning ZLIB_FIND_DEBUG_AND_RELEASE off and using the value of ZLIB_LIBRARY:
${ZLIB_LIBRARY}")
        set(ZLIB_FIND_DEBUG_AND_RELEASE OFF)
    endif (ZLIB_FIND_DEBUG_AND_RELEASE)
else(ZLIB_LIBRARY)
    foreach(search ${_ZLIB_SEARCHES})
      find_library(ZLIB_LIBRARY_RELEASE NAMES ${ZLIB_NAMES} ${${search}} PATH_SUFFIXES lib)
      find_library(ZLIB_LIBRARY_DEBUG NAMES ${ZLIB_NAMES_DEBUG} ${${search}} PATH_SUFFIXES lib)
    endforeach()
    include(SelectLibraryConfigurations)
    select_library_configurations(ZLIB)
endif(ZLIB_LIBRARY)

unset(ZLIB_NAMES)
unset(ZLIB_NAMES_DEBUG)

mark_as_advanced(ZLIB_INCLUDE_DIR)

if(ZLIB_INCLUDE_DIR AND EXISTS "${ZLIB_INCLUDE_DIR}/zlib.h")
    file(STRINGS "${ZLIB_INCLUDE_DIR}/zlib.h" ZLIB_H REGEX "^#define ZLIB_VERSION \"[^\"]*\"$")

    string(REGEX REPLACE "^.*ZLIB_VERSION \"([0-9]+).*$" "\\1" ZLIB_VERSION_MAJOR "${ZLIB_H}")
    string(REGEX REPLACE "^.*ZLIB_VERSION \"[0-9]+\\.([0-9]+).*$" "\\1" ZLIB_VERSION_MINOR  "${ZLIB_H}")
    string(REGEX REPLACE "^.*ZLIB_VERSION \"[0-9]+\\.[0-9]+\\.([0-9]+).*$" "\\1" ZLIB_VERSION_PATCH "${ZLIB_H}")
    set(ZLIB_VERSION_STRING "${ZLIB_VERSION_MAJOR}.${ZLIB_VERSION_MINOR}.${ZLIB_VERSION_PATCH}")

    # only append a TWEAK version if it exists:
    set(ZLIB_VERSION_TWEAK "")
    if( "${ZLIB_H}" MATCHES "ZLIB_VERSION \"[0-9]+\\.[0-9]+\\.[0-9]+\\.([0-9]+)")
        set(ZLIB_VERSION_TWEAK "${CMAKE_MATCH_1}")
        string(APPEND ZLIB_VERSION_STRING ".${ZLIB_VERSION_TWEAK}")
    endif()

    set(ZLIB_MAJOR_VERSION "${ZLIB_VERSION_MAJOR}")
    set(ZLIB_MINOR_VERSION "${ZLIB_VERSION_MINOR}")
    set(ZLIB_PATCH_VERSION "${ZLIB_VERSION_PATCH}")
endif()

if(ZLIB_FIND_DEBUG_AND_RELEASE)
    set(ZLIB_REQUIRED_LIB_VARS ZLIB_LIBRARY_RELEASE ZLIB_LIBRARY_DEBUG)
else(ZLIB_FIND_DEBUG_AND_RELEASE)
    set(ZLIB_REQUIRED_LIB_VARS ZLIB_LIBRARY)
endif(ZLIB_FIND_DEBUG_AND_RELEASE)

include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ZLIB REQUIRED_VARS
    ${ZLIB_REQUIRED_LIB_VARS} ZLIB_INCLUDE_DIR
    VERSION_VAR ZLIB_VERSION_STRING
)

if(ZLIB_FOUND)
    set(ZLIB_INCLUDE_DIRS ${ZLIB_INCLUDE_DIR})

    if(NOT ZLIB_LIBRARIES)
      set(ZLIB_LIBRARIES ${ZLIB_LIBRARY})
    endif()

    if(NOT TARGET ZLIB::ZLIB)
      add_library(ZLIB::ZLIB UNKNOWN IMPORTED)
      set_target_properties(ZLIB::ZLIB PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${ZLIB_INCLUDE_DIRS}")

      if(ZLIB_LIBRARY_RELEASE)
        set_property(TARGET ZLIB::ZLIB APPEND PROPERTY
          IMPORTED_CONFIGURATIONS RELEASE)
        set_target_properties(ZLIB::ZLIB PROPERTIES
          IMPORTED_LOCATION_RELEASE "${ZLIB_LIBRARY_RELEASE}")
      endif()

      if(ZLIB_LIBRARY_DEBUG)
        set_property(TARGET ZLIB::ZLIB APPEND PROPERTY
          IMPORTED_CONFIGURATIONS DEBUG)
        set_target_properties(ZLIB::ZLIB PROPERTIES
          IMPORTED_LOCATION_DEBUG "${ZLIB_LIBRARY_DEBUG}")
      endif()

      if(NOT ZLIB_LIBRARY_RELEASE AND NOT ZLIB_LIBRARY_DEBUG)
        set_property(TARGET ZLIB::ZLIB APPEND PROPERTY
          IMPORTED_LOCATION "${ZLIB_LIBRARY}")
      endif()
    endif()
else(ZLIB_FOUND)
    unset(ZLIB_LIBRARY)
endif(ZLIB_FOUND)
