#.rst:
# FindNode
# --------
#
# Find Node (Node.js)
#
# This module finds if Node.js is installed and determines where 
#   executables are.
#
# The following variables are set if Node.js is found.
# ``Node_EXECUTABLE``
#   full path to the node.js interpreter binary
# ``Node_NPM_EXECUTABLE``
#   full path to the node.js package manager
# ``Node_VERSION``
#   the version of node.js which was found, e.g. "11.13.0"
# ``Node_FOUND``
#   set to true if node.js was found successfully

include(FindPackageHandleStandardArgs)
include(${CMAKE_CURRENT_LIST_DIR}/SMScriptableUtil.cmake)

find_program(Node_EXECUTABLE NAMES node nodejs
    HINTS ENV NODE_DIR PATH_SUFFIXES bin DOC "Node.js interpreter")

find_program(Node_NPM_EXECUTABLE npm.cmd npm
    HINTS ENV NODE_DIR PATH_SUFFIXES bin DOC "Node.js package manager")

mark_as_advanced(
    Node_EXECUTABLE 
    Node_NPM_EXECUTABLE 
)

if (Node_EXECUTABLE)
    execute_process(
        COMMAND ${Node_EXECUTABLE} --version
        OUTPUT_VARIABLE _VERSION
    )
    sm_extract_version(${_VERSION} Node)
endif (Node_EXECUTABLE)

find_package_handle_standard_args(Node
    REQUIRED_VARS Node_EXECUTABLE Node_NPM_EXECUTABLE 
    VERSION_VAR Node_VERSION
)
