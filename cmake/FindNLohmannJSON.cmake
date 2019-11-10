find_path(NLohmannJSON_INCLUDE_DIRS nlohmann/json.hpp)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(NLohmannJSON
    DEFAULT_MSG 
    NLohmannJSON_INCLUDE_DIRS
)

mark_as_advanced(NLohmannJSON_INCLUDE_DIRS)
