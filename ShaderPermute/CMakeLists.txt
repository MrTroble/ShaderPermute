# CMakeList.txt : CMake project for ShaderPermute, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (ShaderPermute "ShaderPermuteTest.cpp" "ShaderPermute.hpp" "json.hpp" "ShaderPermute.cpp" "basicTest.json")
file(COPY basicTest.json DESTINATION ${ShaderPermute_BINARY_DIR})
add_subdirectory(glslang)
target_link_libraries(ShaderPermute PUBLIC glslang SPIRV)

target_include_directories(ShaderPermute PUBLIC ${glslang_SOURCE_DIR})