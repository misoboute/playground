set(JSON_SPIRIT_SOURCES
    json_spirit_reader.cpp
    json_spirit_value.cpp
    json_spirit_writer.cpp
)

SET(JSON_SPIRIT_HEADERS
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

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_definitions(-DJSON_SPIRIT_VALUE_ENABLED)

if(JSON_SPIRIT_WVALUE_ENABLED)
    add_definitions(-DJSON_SPIRIT_WVALUE_ENABLED)
endif(JSON_SPIRIT_WVALUE_ENABLED)

if(JSON_SPIRIT_MVALUE_ENABLED)
    add_definitions(-DJSON_SPIRIT_MVALUE_ENABLED)
endif(JSON_SPIRIT_MVALUE_ENABLED)

if(JSON_SPIRIT_WMVALUE_ENABLED)
    add_definitions(-DJSON_SPIRIT_WMVALUE_ENABLED)
endif(JSON_SPIRIT_WMVALUE_ENABLED)

add_Library(json_spirit ${JSON_SPIRIT_HEADERS} ${JSON_SPIRIT_SOURCES})

include(GNUInstallDirs)
install(TARGETS json_spirit DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${JSON_SPIRIT_HEADERS}
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/json_spirit)
