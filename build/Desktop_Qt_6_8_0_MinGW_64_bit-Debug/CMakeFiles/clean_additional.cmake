# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Musikspieler_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Musikspieler_autogen.dir\\ParseCache.txt"
  "Musikspieler_autogen"
  )
endif()
