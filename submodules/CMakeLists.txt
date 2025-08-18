cmake_minimum_required (VERSION 3.20)

set(BUILD_EXTERNAL OFF)
set(ALLOW_EXTERNAL_SPIRV_TOOLS ON)
set(ENABLE_OPT ON)

add_subdirectory("SPIRV-Headers")
add_subdirectory("SPIRV-Tools")
add_subdirectory("glslang")
