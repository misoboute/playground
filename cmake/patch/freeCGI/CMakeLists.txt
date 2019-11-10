# Created by Swissmeda. freeCGI does not provide its own CMake based build
# system at this point. This 

cmake_minimum_required(VERSION 2.8)

project(freeCGI)

set(FREECGI_HEADERS
    include/a_base.h
    include/a_cgi.h
    include/a_conv.h
    include/a_elist.h
    include/a_form.h
    include/a_global.h
    include/a_helper.h
    include/a_html.h
    include/a_list.h
    include/a_predef.h
    include/a_random.h
    include/a_scart.h
    include/a_stream.h
    include/a_struct.h
    include/freeCGI.h
)

set(FREECGI_SOURCES
    src/a_ansi.cpp
    src/a_base.cpp
    src/a_cgi.cpp
    src/a_conv.cpp
    src/a_cookie.cpp
    src/a_crypto.cpp
    src/a_datai.cpp
    src/a_elem.cpp
    src/a_elist.cpp
    src/a_flist.cpp
    src/a_formi.cpp
    src/a_global.cpp
    src/a_helper.cpp
    src/a_html.cpp
    src/a_list.cpp
    src/a_pairi.cpp
    src/a_plist.cpp
    src/a_random.cpp
    src/a_scart.cpp
    src/a_scarti.cpp
    src/a_stream.cpp
    src/a_url.cpp
)

include_directories(include)

if (WIN32)
    add_definitions(-DWIN32 -D_LIB -D_CRT_SECURE_NO_WARNINGS)
elseif (APPLE)
    add_definitions(-DMACOS -D_CPPRTTI)
endif (WIN32)

if (CMAKE_VERSION VERSION_GREATER_EQUAL 3.12)
    if (WIN32)
        add_compile_definitions($<$<CONFIG:Debug>:_DEBUG>)
    elseif (APPLE)
        add_compile_definitions($<$<CONFIG:Debug>:DEBUG_DUMP>)
    endif (WIN32)
endif (CMAKE_VERSION VERSION_GREATER_EQUAL 3.12)

set(CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/PDBFilesDebug)

add_library(freeCGI ${FREECGI_HEADERS} ${FREECGI_SOURCES})

install(TARGETS freeCGI DESTINATION lib)
install(FILES ${CMAKE_COMPILE_PDB_OUTPUT_DIRECTORY_DEBUG}/freeCGI.pdb
    DESTINATION lib OPTIONAL)
install(FILES ${FREECGI_HEADERS} DESTINATION include/freeCGI)
