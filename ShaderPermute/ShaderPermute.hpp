#pragma once

#include <format>
#include <functional>
#include <map>
#include <string>
#include <vector>

#ifndef SPR_NO_FSTREAM
#include <fstream>
#endif

#ifndef SPR_NO_JSON_HPP_INCLUDE
#include "json.hpp"
#endif

#define SPR_OPTIONAL_FROM(v1)                                                  \
  const auto eItr##v1 = end(nlohmann_json_j);                                  \
  const auto itr##v1 = nlohmann_json_j.find(#v1);                              \
  if (eItr##v1 != itr##v1)                                                     \
    (*itr##v1).get_to(nlohmann_json_t.v1);

#define SPR_OPTIONAL_TO(v1)                                                    \
  if (!((bool)nlohmann_json_t.v1)) {                                           \
    NLOHMANN_JSON_TO(v1);                                                      \
  }

#define SPR_OPTIONAL_TO_L(v1)                                                  \
  if (!((bool)nlohmann_json_t.v1.empty())) {                                   \
    NLOHMANN_JSON_TO(v1);                                                      \
  }

#if !defined(SPR_NO_GLSL) && !defined(SPR_NO_GLSL_INCLUDE)
#include <SPIRV/GlslangToSpv.h>
#include <glslang/Public/ShaderLang.h>
#endif

#ifndef SPR_NO_GLSL
NLOHMANN_JSON_SERIALIZE_ENUM(EShLanguage,
                             {{EShLangVertex, "vertex"},
                              {EShLangTessControl, "tesslation-control"},
                              {EShLangTessEvaluation, "tesslation-evaluation"},
                              {EShLangGeometry, "geometry"},
                              {EShLangFragment, "fragment"},
                              {EShLangCompute, "compute"},
                              {EShLangRayGen, "raygen"},
                              {EShLangIntersect, "intersect"},
                              {EShLangAnyHit, "anyhit"},
                              {EShLangClosestHit, "closehit"},
                              {EShLangMiss, "miss"},
                              {EShLangCallable, "callable"},
                              {EShLangTaskNV, "tasknv"},
                              {EShLangMeshNV, "meshnv"}

                             });

namespace glslang {
NLOHMANN_JSON_SERIALIZE_ENUM(EShClient, {{EShClientNone, "none"},
                                         {EShClientVulkan, "vulkan"},
                                         {EShClientOpenGL, "opengl"}});
NLOHMANN_JSON_SERIALIZE_ENUM(EShTargetClientVersion,
                             {{EShTargetVulkan_1_0, "vulkan_1_0"},
                              {EShTargetVulkan_1_1, "vulkan_1_1"},
                              {EShTargetVulkan_1_2, "vulkan_1_2"},
                              {EShTargetOpenGL_450, "opengl_450"}});
NLOHMANN_JSON_SERIALIZE_ENUM(EShTargetLanguage,
                             {{EShTargetNone, "none"}, {EShTargetSpv, "spv"}});
NLOHMANN_JSON_SERIALIZE_ENUM(EShTargetLanguageVersion,
                             {
                                 {EShTargetSpv_1_0, "spv_1_0"},
                                 {EShTargetSpv_1_1, "spv_1_1"},
                                 {EShTargetSpv_1_2, "spv_1_2"},
                                 {EShTargetSpv_1_3, "spv_1_3"},
                                 {EShTargetSpv_1_4, "spv_1_4"},
                                 {EShTargetSpv_1_5, "spv_1_5"},
                             });
} // namespace glslang
#endif

namespace permute {

constexpr TBuiltInResource defaultTBuiltInResource = {
    /* .MaxLights = */ 32,
    /* .MaxClipPlanes = */ 6,
    /* .MaxTextureUnits = */ 32,
    /* .MaxTextureCoords = */ 32,
    /* .MaxVertexAttribs = */ 64,
    /* .MaxVertexUniformComponents = */ 4096,
    /* .MaxVaryingFloats = */ 64,
    /* .MaxVertexTextureImageUnits = */ 32,
    /* .MaxCombinedTextureImageUnits = */ 80,
    /* .MaxTextureImageUnits = */ 32,
    /* .MaxFragmentUniformComponents = */ 4096,
    /* .MaxDrawBuffers = */ 32,
    /* .MaxVertexUniformVectors = */ 128,
    /* .MaxVaryingVectors = */ 8,
    /* .MaxFragmentUniformVectors = */ 16,
    /* .MaxVertexOutputVectors = */ 16,
    /* .MaxFragmentInputVectors = */ 15,
    /* .MinProgramTexelOffset = */ -8,
    /* .MaxProgramTexelOffset = */ 7,
    /* .MaxClipDistances = */ 8,
    /* .MaxComputeWorkGroupCountX = */ 65535,
    /* .MaxComputeWorkGroupCountY = */ 65535,
    /* .MaxComputeWorkGroupCountZ = */ 65535,
    /* .MaxComputeWorkGroupSizeX = */ 1024,
    /* .MaxComputeWorkGroupSizeY = */ 1024,
    /* .MaxComputeWorkGroupSizeZ = */ 64,
    /* .MaxComputeUniformComponents = */ 1024,
    /* .MaxComputeTextureImageUnits = */ 16,
    /* .MaxComputeImageUniforms = */ 8,
    /* .MaxComputeAtomicCounters = */ 8,
    /* .MaxComputeAtomicCounterBuffers = */ 1,
    /* .MaxVaryingComponents = */ 60,
    /* .MaxVertexOutputComponents = */ 64,
    /* .MaxGeometryInputComponents = */ 64,
    /* .MaxGeometryOutputComponents = */ 128,
    /* .MaxFragmentInputComponents = */ 128,
    /* .MaxImageUnits = */ 8,
    /* .MaxCombinedImageUnitsAndFragmentOutputs = */ 8,
    /* .MaxCombinedShaderOutputResources = */ 8,
    /* .MaxImageSamples = */ 0,
    /* .MaxVertexImageUniforms = */ 0,
    /* .MaxTessControlImageUniforms = */ 0,
    /* .MaxTessEvaluationImageUniforms = */ 0,
    /* .MaxGeometryImageUniforms = */ 0,
    /* .MaxFragmentImageUniforms = */ 8,
    /* .MaxCombinedImageUniforms = */ 8,
    /* .MaxGeometryTextureImageUnits = */ 16,
    /* .MaxGeometryOutputVertices = */ 256,
    /* .MaxGeometryTotalOutputComponents = */ 1024,
    /* .MaxGeometryUniformComponents = */ 1024,
    /* .MaxGeometryVaryingComponents = */ 64,
    /* .MaxTessControlInputComponents = */ 128,
    /* .MaxTessControlOutputComponents = */ 128,
    /* .MaxTessControlTextureImageUnits = */ 16,
    /* .MaxTessControlUniformComponents = */ 1024,
    /* .MaxTessControlTotalOutputComponents = */ 4096,
    /* .MaxTessEvaluationInputComponents = */ 128,
    /* .MaxTessEvaluationOutputComponents = */ 128,
    /* .MaxTessEvaluationTextureImageUnits = */ 16,
    /* .MaxTessEvaluationUniformComponents = */ 1024,
    /* .MaxTessPatchComponents = */ 120,
    /* .MaxPatchVertices = */ 32,
    /* .MaxTessGenLevel = */ 64,
    /* .MaxViewports = */ 16,
    /* .MaxVertexAtomicCounters = */ 0,
    /* .MaxTessControlAtomicCounters = */ 0,
    /* .MaxTessEvaluationAtomicCounters = */ 0,
    /* .MaxGeometryAtomicCounters = */ 0,
    /* .MaxFragmentAtomicCounters = */ 8,
    /* .MaxCombinedAtomicCounters = */ 8,
    /* .MaxAtomicCounterBindings = */ 1,
    /* .MaxVertexAtomicCounterBuffers = */ 0,
    /* .MaxTessControlAtomicCounterBuffers = */ 0,
    /* .MaxTessEvaluationAtomicCounterBuffers = */ 0,
    /* .MaxGeometryAtomicCounterBuffers = */ 0,
    /* .MaxFragmentAtomicCounterBuffers = */ 1,
    /* .MaxCombinedAtomicCounterBuffers = */ 1,
    /* .MaxAtomicCounterBufferSize = */ 16384,
    /* .MaxTransformFeedbackBuffers = */ 4,
    /* .MaxTransformFeedbackInterleavedComponents = */ 64,
    /* .MaxCullDistances = */ 8,
    /* .MaxCombinedClipAndCullDistances = */ 8,
    /* .MaxSamples = */ 4,
    /* .maxMeshOutputVerticesNV = */ 256,
    /* .maxMeshOutputPrimitivesNV = */ 512,
    /* .maxMeshWorkGroupSizeX_NV = */ 32,
    /* .maxMeshWorkGroupSizeY_NV = */ 1,
    /* .maxMeshWorkGroupSizeZ_NV = */ 1,
    /* .maxTaskWorkGroupSizeX_NV = */ 32,
    /* .maxTaskWorkGroupSizeY_NV = */ 1,
    /* .maxTaskWorkGroupSizeZ_NV = */ 1,
    /* .maxMeshViewCountNV = */ 4,
    /* .maxDualSourceDrawBuffersEXT = */ 1,

    /* .limits = */
    {
        /* .nonInductiveForLoops = */ 1,
        /* .whileLoops = */ 1,
        /* .doWhileLoops = */ 1,
        /* .generalUniformIndexing = */ 1,
        /* .generalAttributeMatrixVectorIndexing = */ 1,
        /* .generalVaryingIndexing = */ 1,
        /* .generalSamplerIndexing = */ 1,
        /* .generalVariableIndexing = */ 1,
        /* .generalConstantMatrixVectorIndexing = */ 1,
    }};

using lookup =
    std::map<std::string, std::function<std::string(const std::string &)>>;

enum class ShaderCodeFlags { NONE = 0, REQUIRED = 1 };

NLOHMANN_JSON_SERIALIZE_ENUM(ShaderCodeFlags,
                             {{ShaderCodeFlags::NONE, "none"},
                              {ShaderCodeFlags::REQUIRED, "required"}})

enum class OutputType { ERROR, TEXT, BINARY };

inline bool isRequired(const ShaderCodeFlags flag) {
  return (int)flag & (int)ShaderCodeFlags::REQUIRED;
}

template <class T> inline bool isInDependency(T &dependency, T &dependsOn) {
  const auto endItr = end(dependency);
  for (auto target : dependsOn) {
    auto itr = begin(dependency);
    if (std::find(itr, endItr, target) == endItr)
      return false;
  }
  return true;
}

struct ShaderCodes {
  std::vector<std::string> code;
  ShaderCodeFlags flags = ShaderCodeFlags::NONE;
  std::vector<std::string> dependsOn;

  friend void to_json(nlohmann::json &nlohmann_json_j,
                      const ShaderCodes &nlohmann_json_t) {
    NLOHMANN_JSON_TO(code);
    SPR_OPTIONAL_TO(flags);
    SPR_OPTIONAL_TO_L(dependsOn);
  }

  friend void from_json(const nlohmann::json &nlohmann_json_j,
                        ShaderCodes &nlohmann_json_t) {
    NLOHMANN_JSON_FROM(code);
    SPR_OPTIONAL_FROM(flags);
    SPR_OPTIONAL_FROM(dependsOn);
  }
};

struct GenerateInput {
  const std::vector<ShaderCodes> &codes;
  const std::vector<std::string> &dependencies;
  const nlohmann::json &settings;
};

struct GenerateOutput {
  std::string output;
  OutputType type = OutputType::ERROR;
  std::vector<unsigned int> data;
};

class PermuteText {
public:
  inline static GenerateOutput generate(const GenerateInput input) {
    std::stringstream buffer;
    for (const auto &code : input.codes) {
      if (isRequired(code.flags) ||
          isInDependency(input.dependencies, code.dependsOn)) {
        for (const auto &codePart : code.code)
          buffer << codePart << std::endl;
      }
    }
    return {buffer.str(), OutputType::TEXT};
  }
};

inline void postProcess(std::string &codePart, const lookup &callback) {
  if (callback.empty())
    return;
  auto eItr = end(codePart);
  auto startWordItr = eItr;
  auto paramStartItr = eItr;
  lookup::value_type::second_type func(nullptr);
  for (auto itr = codePart.begin(); itr != eItr; itr++) {
    if (*itr == '$') {
      startWordItr = itr + 1;
      continue;
    }
    if (startWordItr != eItr && *itr == '_') {
      const auto word = std::string(startWordItr, itr);
      const auto fncItr = callback.find(word);
      if (fncItr != end(callback)) {
        func = fncItr->second;
        paramStartItr = itr + 1;
      } else {
        startWordItr = eItr;
      }
      continue;
    }
    if (startWordItr != eItr && *itr == ' ') {
      const std::string param(paramStartItr, itr);
      const auto replace = func(param);
      const auto distance = std::distance(startWordItr, itr);
      codePart = codePart.replace(startWordItr - 1, itr, replace);
      eItr = end(codePart);
      itr = begin(codePart) + distance;
      startWordItr = eItr;
    }
  }
}

#ifndef SPR_NO_GLSL
struct GlslSettings {
  EShLanguage shaderType;
  glslang::EShClient targetClient = glslang::EShClient::EShClientVulkan;
  glslang::EShTargetClientVersion targetVersion =
      glslang::EShTargetClientVersion::EShTargetVulkan_1_0;
  glslang::EShTargetLanguage targetLanguage =
      glslang::EShTargetLanguage::EShTargetSpv;
  glslang::EShTargetLanguageVersion targetLanguageVersion =
      glslang::EShTargetLanguageVersion::EShTargetSpv_1_5;

  friend void to_json(nlohmann::json &nlohmann_json_j,
                      const GlslSettings &nlohmann_json_t) {
    NLOHMANN_JSON_TO(shaderType);
    SPR_OPTIONAL_TO(targetClient);
    SPR_OPTIONAL_TO(targetVersion);
    SPR_OPTIONAL_TO(targetLanguage);
    SPR_OPTIONAL_TO(targetLanguageVersion);
  }

  friend void from_json(const nlohmann::json &nlohmann_json_j,
                        GlslSettings &nlohmann_json_t) {
    NLOHMANN_JSON_FROM(shaderType);
    SPR_OPTIONAL_FROM(targetClient);
    SPR_OPTIONAL_FROM(targetVersion);
    SPR_OPTIONAL_FROM(targetLanguage);
    SPR_OPTIONAL_FROM(targetLanguageVersion);
  }
};

static std::map<std::string, int> lookupCounter;

std::string next(const std::string &input) {
  const auto id = lookupCounter[input];
  lookupCounter[input]++;
  if (input == "ublock")
    return std::format("layout(binding={}) uniform BLOCK{}", id, id);
  return std::format("layout(location={}) {}", id, input);
}

static const lookup glslLookup = {{"next", next}};

class PermuteGLSL {
public:
  inline static GenerateOutput generate(const GenerateInput input) {
    auto output = PermuteText::generate(input);
    if (output.type == OutputType::ERROR)
      return output;
    try {
      glslang::InitializeProcess();
      postProcess(output.output, glslLookup);
      const auto stringPtr = output.output.c_str();
      const GlslSettings settings = input.settings.get<GlslSettings>();
      glslang::TShader shader(settings.shaderType);
      shader.setStrings(&stringPtr, 1);
      shader.setEnvInput(glslang::EShSourceGlsl, settings.shaderType,
                         settings.targetClient, 100);
      shader.setEnvClient(settings.targetClient, settings.targetVersion);
      shader.setEnvTarget(settings.targetLanguage,
                          settings.targetLanguageVersion);
      if (!shader.parse(&defaultTBuiltInResource, 450, true,
                        EShMessages::EShMsgVulkanRules)) {
        return {shader.getInfoLog(), OutputType::ERROR};
      }
      const auto interm = shader.getIntermediate();
      std::vector<unsigned int> outputData;
      glslang::GlslangToSpv(*interm, outputData);
      glslang::FinalizeProcess();
      return {output.output, OutputType::BINARY, outputData};
    } catch (const std::exception &) {
      glslang::FinalizeProcess();
      return {"Could not parse glsl settings!", OutputType::ERROR};
    }
    return {};
  }
};

#endif

template <class T> class Permute {

private:
  std::vector<ShaderCodes> codes;
  nlohmann::json settings;
  GenerateOutput output;

public:
  inline bool generate(const std::vector<std::string> &dependencies = {}) {
    const GenerateInput input = {codes, dependencies, settings};
    output = T::generate(input);
    return success();
  }

  inline bool success() { return output.type != OutputType::ERROR; }

  inline std::string getContent() { return output.output; }

  friend void to_json(nlohmann::json &nlohmann_json_j,
                      const Permute &nlohmann_json_t) {
    NLOHMANN_JSON_TO(codes);
    SPR_OPTIONAL_TO_L(settings);
  }

  friend void from_json(const nlohmann::json &nlohmann_json_j,
                        Permute &nlohmann_json_t) {
    NLOHMANN_JSON_FROM(codes);
    SPR_OPTIONAL_FROM(settings);
  }
};

template <class T> inline Permute<T> fromJson(nlohmann::json json) {
  return json.get<Permute<T>>();
}

#ifndef SPR_NO_FSTREAM
template <class T> inline Permute<T> fromFile(std::string path) {
  std::ifstream inputfile(path);
  if (!inputfile.good())
    throw std::runtime_error("File not found!");
  nlohmann::json json;
  inputfile >> json;
  return fromJson<T>(json);
}
#endif

} // namespace permute
