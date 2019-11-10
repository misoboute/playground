# This module creates targets for retrieving, building, and installing of 
# Inja engine (single header).
if(SM_RETRIEVE_INJA_INCLUDED)
	return()
endif(SM_RETRIEVE_INJA_INCLUDED)
set(SM_RETRIEVE_INJA_INCLUDED 1)

include(SMUtil)

set(_INJA_DESTINATION_FILE ${SM_3RDPARTY_INSTALL_DIR}/include/inja/inja.hpp)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
set(_INJA_EXPECTED_HASH 9feeb0e742cd37c92c4d22b2f5835df162751eb6)
set(_INJA_DOWNLOAD_URL 
    https://raw.githubusercontent.com/DanmerZ/inja/fix_vs2015_build_error/single_include/inja/inja.hpp)
	
message("Downloading inja
    URL: ${_INJA_DOWNLOAD_URL}
    Destination file: ${_INJA_DESTINATION_FILE}
    Expected SHA1: ${_INJA_EXPECTED_HASH}"
)

file(DOWNLOAD
    ${_INJA_DOWNLOAD_URL}
    ${_INJA_DESTINATION_FILE}
    SHOW_PROGRESS
    EXPECTED_HASH SHA1=${_INJA_EXPECTED_HASH}
)
