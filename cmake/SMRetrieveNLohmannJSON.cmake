# This module creates targets for retrieving, building, and installing of 
# NLohmann JSON (single header).
if(SM_RETRIEVE_BOOST_INCLUDED)
    return()
endif(SM_RETRIEVE_BOOST_INCLUDED)
set(SM_RETRIEVE_BOOST_INCLUDED 1)

include(SwissMedaUtil)

set(_NLOHMANN_JSON_DESTINATION_FILE
    ${SWISSMEDA_3RDPARTY_INSTALL_DIR}/include/nlohmann/json.hpp)

# The URL HASH created using online facility:
# 	https://hash.online-convert.com/sha1-generator
# It helps CMake verify the integrity of the file and not download it again.
set(_NLOHMANN_JSON_EXPECTED_HASH 9963171833fd4a5caa988b3d1dea9672a1639c9c)

set(_NLOHMANN_JSON_DOWNLOAD_URL
    https://github.com/nlohmann/json/releases/download/v3.6.1/json.hpp)

message("Downloading NLohmann JSON
    URL: ${_NLOHMANN_JSON_DOWNLOAD_URL}
    Destination file: ${_NLOHMANN_JSON_DESTINATION_FILE}
    Expected SHA1: ${_NLOHMANN_JSON_EXPECTED_HASH}"
)

file(DOWNLOAD
    ${_NLOHMANN_JSON_DOWNLOAD_URL}
    ${_NLOHMANN_JSON_DESTINATION_FILE}
    SHOW_PROGRESS
    EXPECTED_HASH SHA1=${_NLOHMANN_JSON_EXPECTED_HASH}
)
