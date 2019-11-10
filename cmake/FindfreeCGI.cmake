find_path(freeCGI_INCLUDE_DIRS freeCGI.h PATH_SUFFIXES freeCGI)

find_library(freeCGI_LIBRARY_RELEASE NAMES freeCGI)
find_library(freeCGI_LIBRARY_DEBUG NAMES freeCGId)

set(freeCGI_LIBRARIES 
    optimized ${freeCGI_LIBRARY_RELEASE} 
    debug ${freeCGI_LIBRARY_DEBUG})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(freeCGI 
    DEFAULT_MSG 
    freeCGI_INCLUDE_DIRS 
    freeCGI_LIBRARY_RELEASE freeCGI_LIBRARY_DEBUG
)

mark_as_advanced(
    freeCGI_INCLUDE_DIRS
    freeCGI_LIBRARY_RELEASE
    freeCGI_LIBRARY_DEBUG
    freeCGI_LIBRARIES
)
