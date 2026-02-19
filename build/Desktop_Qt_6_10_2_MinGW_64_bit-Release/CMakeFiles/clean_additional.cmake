# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\surebetcalc_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\surebetcalc_autogen.dir\\ParseCache.txt"
  "surebetcalc_autogen"
  )
endif()
