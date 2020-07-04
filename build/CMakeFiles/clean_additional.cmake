# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "CMakeFiles/pixeleditor_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/pixeleditor_autogen.dir/ParseCache.txt"
  "pixeleditor_autogen"
  )
endif()
