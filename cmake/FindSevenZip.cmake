find_program(SevenZip_7z 7z
    PATH_SUFFIXES 7-Zip DOC "Path to 7-Zip executable")

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SevenZip DEFAULT_MSG SevenZip_7z)

# Based on the location of *_INLCUDE_DIRS, sm_find_or_get decides whether it
# has found the right package (built by SMUtil):
get_filename_component(SevenZip_INCLUDE_DIRS ${SevenZip_7z} DIRECTORY)

mark_as_advanced(SevenZip_7z)
