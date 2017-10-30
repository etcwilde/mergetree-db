
# Add Cpp files from current directory
SET(PROJECT_SOURCE_LIST
  "${PROJECT_SOURCE_LIST}"
  "${CMAKE_CURRENT_SOURCE_DIR}/mergetree.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/sqlite3.hpp"
  PARENT_SCOPE)
