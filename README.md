# ShaderPermute

[![CMake](https://github.com/MrTroble/ShaderPermute/actions/workflows/cmake.yml/badge.svg)](https://github.com/MrTroble/ShaderPermute/actions/workflows/cmake.yml)

Simple library to generate shader permutations based on dependencies. Can also be used for text.

## Current features

* Generate permutations from a json
* Generate SPIR-V code based on glsl code from the given permutations
* Post-process glsl code before compilation via lookups

## Example

if you want the integrated glsl compiler use the `permute::PermuteGLSL` template

```cpp
  auto &perm = permute::fromFile<permute::PermuteGLSL>("shader.json");
```

for just text with no lookups use `permute::PermuteText`

```cpp
  auto &perm = permute::fromFile<permute::PermuteText>("text.json");
```

loading from memory is also supported via `permute::fromJson`

```cpp
  auto &perm = permute::fromJson<PermuteText/GLSL>(js);
```

use the generate function to generate a permutation based on some dependencies.
Keep in mind that you need to call `glslang::InitializeProcess()` and `glslang::FinalizeProcess()`

```cpp
bool success = perm.generate(); // Generates SPIR-V/Text without dependencies
```

```cpp
bool success = perm.generate({"COLOR"}) // Generates SPIR-V/Text with the COLOR dependency
```

Use the getter methods to retrieve the needed information afterwards

## Json structure

```json
{
  "settings": { // mandatory while using glsl
    "shaderType": "vertex" // mandatory while using glsl
  },
  "codes": [
    {
      "code": [
        "$next_in vec3 POSITION;",
        "$next_out vec3 POSITIONOUT;"
      ]
    },
    {
      "code": [
        "$next_in vec4 COLOR;", // builtin lookup gives it the next input layout qualifier (or binding for ubos)
        "$next_out vec4 COLOROUT;"
      ],
      "dependsOn":  ["COLOR"]
    },
    {
      "code": [
        "$next_ublock { mat4 model; } ublock_0;",
        "$next_ublock { mat4 vp;} ublock_1;",
        "out gl_PerVertex { vec4 gl_Position; };",
        "void main() {",
        "gl_Position = ublock_1.vp * ublock_0.model * vec4(POSITION, 1);",
        "POSITIONOUT = gl_Position.xyz;"
      ]
    },
    {
      "code": [
        "COLOROUT = COLOR;"
      ],
      "dependsOn": ["COLOR"]
    },
    {
      "code": [
        "}"
      ]
    }
  ]
}
```
