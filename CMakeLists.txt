# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

SET(CMAKE_CXX_STANDARD 23)

project ("ShaderPermute")
include(CTest)
# Include sub-projects.
add_subdirectory ("ShaderPermute/ShaderPermuteTest")
