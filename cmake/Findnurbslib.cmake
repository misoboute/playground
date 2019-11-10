find_path(nurbslib_INCLUDE_DIRS nurbslib.h PATH_SUFFIXES nurbslib)

find_library(nurbslib_LIBRARY_RELEASE NAMES nurbslib)
find_library(nurbslib_LIBRARY_DEBUG NAMES nurbslibd)

set(nurbslib_LIBRARIES 
    optimized ${nurbslib_LIBRARY_RELEASE} 
    debug ${nurbslib_LIBRARY_DEBUG})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(nurbslib 
    DEFAULT_MSG 
    nurbslib_INCLUDE_DIRS 
    nurbslib_LIBRARY_RELEASE nurbslib_LIBRARY_DEBUG
)

mark_as_advanced(
    nurbslib_INCLUDE_DIRS
    nurbslib_LIBRARY_RELEASE
    nurbslib_LIBRARY_DEBUG
    nurbslib_LIBRARIES
)
