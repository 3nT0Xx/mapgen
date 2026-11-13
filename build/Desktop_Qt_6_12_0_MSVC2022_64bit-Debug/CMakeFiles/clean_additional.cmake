# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\mapgen_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\mapgen_autogen.dir\\ParseCache.txt"
  "mapgen_autogen"
  )
endif()
