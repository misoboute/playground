find_path(JSONSpirit_INCLUDE_DIRS json_spirit.h PATH_SUFFIXES json_spirit)

set(_JSON_SPIRIT_HEADERS
    json_spirit_reader.h
    json_spirit_value.h
    json_spirit_writer.h
    json_spirit.h
    json_spirit_error_position.h
    json_spirit_reader_template.h
    json_spirit_stream_reader.h
    json_spirit_utils.h
    json_spirit_writer_options.h
    json_spirit_writer_template.h 
)

foreach(_JSON_SPIRIT_HEADER ${_JSON_SPIRIT_HEADERS})
    if (NOT EXISTS ${json_spirit_INCLUDE_DIR}/${_JSON_SPIRIT_HEADER})
        unset(JSONSpirit_INCLUDE_DIRS)
    endif (NOT EXISTS ${json_spirit_INCLUDE_DIR}/${_JSON_SPIRIT_HEADER})
endforeach(_JSON_SPIRIT_HEADER)

find_library(JSONSpirit_LIBRARY_RELEASE NAMES json_spirit)
find_library(JSONSpirit_LIBRARY_DEBUG NAMES json_spiritd)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(JSONSpirit 
    DEFAULT_MSG 
    JSONSpirit_INCLUDE_DIRS JSONSpirit_LIBRARY_RELEASE JSONSpirit_LIBRARY_DEBUG
)

if (JSONSpirit_FOUND)
    set(JSONSpirit_LIBRARIES 
        optimized ${JSONSpirit_LIBRARY_RELEASE} 
        debug ${JSONSpirit_LIBRARY_DEBUG}
    )
endif (JSONSpirit_FOUND)

mark_as_advanced(
    JSONSpirit_INCLUDE_DIRS
    JSONSpirit_LIBRARY_RELEASE
    JSONSpirit_LIBRARY_DEBUG
)
