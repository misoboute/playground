find_path(Inja_INCLUDE_DIRS inja/inja.hpp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Inja
    DEFAULT_MSG
	Inja_INCLUDE_DIRS
)

mark_as_advanced(Inja_INCLUDE_DIRS)
