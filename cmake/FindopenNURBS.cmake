find_path(openNURBS_INCLUDE_DIRS opennurbs.h)

find_library(OPENNURBS_LIBRARY_RELEASE NAMES opennurbs)
find_library(OPENNURBS_LIBRARY_DEBUG NAMES opennurbsd)

set(openNURBS_LIBRARIES 
    optimized ${OPENNURBS_LIBRARY_RELEASE} 
    debug ${OPENNURBS_LIBRARY_DEBUG})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(openNURBS 
    DEFAULT_MSG 
    openNURBS_INCLUDE_DIRS 
    OPENNURBS_LIBRARY_RELEASE OPENNURBS_LIBRARY_DEBUG
)

mark_as_advanced(
    openNURBS_INCLUDE_DIRS
    OPENNURBS_LIBRARY_RELEASE
    OPENNURBS_LIBRARY_DEBUG
    openNURBS_LIBRARIES
)
