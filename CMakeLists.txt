# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

SET(CMAKE_CXX_STANDARD 23)

project ("ShaderPermute")
include(CTest)
include(FetchContent)

set(ENABLE_CTEST OFF CACHE BOOL "" FORCE)
set(ENABLE_HLSL OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
  glslang
  URL https://github.com/KhronosGroup/glslang/archive/c9706bdda0ac22b9856f1aa8261e5b9e15cd20c5.zip
)

FetchContent_Declare(
  materialx
  URL https://github.com/materialx/MaterialX/releases/download/v1.38.3/MaterialX_Windows_VS2022_x64_Python39.zip
)

# Include sub-projects.
add_subdirectory ("ShaderPermute/ShaderPermuteTest")

