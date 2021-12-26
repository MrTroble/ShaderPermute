cmake_minimum_required (VERSION 3.8)

add_executable (ShaderPermuteTest "ShaderPermuteTest.cpp")
file(COPY basicTest.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
add_subdirectory("../glslang" "glslang")

# The commands to add to your own project for glsl compiler support
target_link_libraries(ShaderPermuteTest PUBLIC glslang SPIRV)
target_include_directories(ShaderPermuteTest PUBLIC ${glslang_SOURCE_DIR})