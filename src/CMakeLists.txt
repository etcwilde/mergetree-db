
# Add Cpp files from current directory
SET(PROJECT_SOURCE_LIST
  "${PROJECT_SOURCE_LIST}"
  "${CMAKE_CURRENT_SOURCE_DIR}/mergetree.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/graph.hpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/graph.cpp"
  PARENT_SCOPE)
