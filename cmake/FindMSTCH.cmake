find_path(MSTCH_INCLUDE_DIRS mstch/mstch.hpp)

find_library(MSTCH_LIBRARY_RELEASE NAMES mstch)
find_library(MSTCH_LIBRARY_DEBUG NAMES mstchd)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(MSTCH
	DEFAULT_MSG
	MSTCH_INCLUDE_DIRS
	MSTCH_LIBRARY_RELEASE MSTCH_LIBRARY_DEBUG
)
	
if (MSTCH_FOUND)
    set(MSTCH_LIBRARIES 
        optimized ${MSTCH_LIBRARY_RELEASE} 
        debug ${MSTCH_LIBRARY_DEBUG}
    )
endif (MSTCH_FOUND)

mark_as_advanced(
	MSTCH_INCLUDE_DIRS
	MSTCH_LIBRARY_RELEASE
	MSTCH_LIBRARY_DEBUG
	MSTCH_LIBRARIES
)