# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\CodeEditor_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\CodeEditor_autogen.dir\\ParseCache.txt"
  "CodeEditor_autogen"
  )
endif()
