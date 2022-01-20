cmake_minimum_required (VERSION 3.8)

add_executable (ShaderPermuteTest "ShaderPermuteTest.cpp")
file(COPY basicTest.json DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)

target_link_libraries(ShaderPermuteTest PUBLIC gtest_main)

# The commands to add to your own project for glsl compiler support
target_link_libraries(ShaderPermuteTest PUBLIC glslang SPIRV)
target_include_directories(ShaderPermuteTest PUBLIC ${glslang_SOURCE_DIR} ${materialx_SOURCE_DIR}/include)

gtest_add_tests(TARGET ShaderPermuteTest)
