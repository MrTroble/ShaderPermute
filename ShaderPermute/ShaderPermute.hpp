#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>

#ifndef SPR_NO_JSON_HPP_INCLUDE
#include "json.hpp"
#endif

#if !defined (SPR_NO_GLSL) && !defined (SPR_NO_GLSL_INCLUDE)
#include <glslang/Public/ShaderLang.h>
#include <SPIRV/GlslangToSpv.h>
#endif

namespace permute {

	constexpr TBuiltInResource defaultTBuiltInResource;

	enum class InputType {
		REQUIRED = 1
	};

	enum class OutputType {
		ERROR,
		TEXT,
		BINARY
	};

	inline bool isRequired(const uint32_t flag) {
		return flag & (int)InputType::REQUIRED;
	}

	template<class T>
	inline bool isInDependency(T& dependency, T& dependsOn) {
		for (auto target : dependsOn) {
			auto itr = begin(dependency);
			const auto endItr = end(dependency);
			if (std::find(itr, endItr, target) == endItr)
				return false;
		}
		return true;
	}

	struct ShaderCodes {
		std::vector<std::string> code;
		unsigned int flags;
		std::vector<std::string> dependsOn;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(ShaderCodes, code, dependsOn, flags)
	};

	struct GenerateInput {
		const std::vector<ShaderCodes>& codes;
		const std::vector<std::string>& dependencies;
		const nlohmann::json& settings;
		const std::map<std::string, std::function<std::string()>>& translationTable;
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
			for (const auto& code : input.codes) {
				if (isRequired(code.flags) || isInDependency(input.dependencies, code.dependsOn)) {
				}
			}
			return { buffer.str(), OutputType::TEXT };
		}
	};

#ifndef SPR_NO_GLSL
	struct GlslSettings {
		EShLanguage shaderType;
		glslang::EShClient targetClient;
		glslang::EShTargetClientVersion targetVersion;
		glslang::EShTargetLanguage targetLanguage;
		glslang::EShTargetLanguageVersion targetLanguageVersion;

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(GlslSettings, shaderType, targetClient, targetVersion, targetLanguage, targetLanguageVersion)
	};

	class PermuteGLSL {
	public:
		inline static GenerateOutput generate(const GenerateInput input) {
			const auto output = PermuteText::generate(input);
			if (output.type == OutputType::ERROR)
				return output;
			try
			{
				const GlslSettings settings = input.settings.get<GlslSettings>();
				glslang::TShader shader(settings.shaderType);
				const auto stringPtr = output.output.c_str();
				shader.setStrings(&stringPtr, 1);
				shader.setEnvInput(glslang::EShSourceGlsl, settings.shaderType, settings.targetClient, 100);
				shader.setEnvClient(settings.targetClient, settings.targetVersion);
				shader.setEnvTarget(settings.targetLanguage, settings.targetLanguageVersion);
				if (!shader.parse(&defaultTBuiltInResource, 450, true, EShMessages::EShMsgVulkanRules)) {
					return { shader.getInfoLog(), OutputType::ERROR };
				}
				const auto interm = shader.getIntermediate();
				std::vector<unsigned int> outputData;
				glslang::GlslangToSpv(*interm, outputData);
			}
			catch (const std::exception&)
			{
				return { "Could not parse glsl settings!", OutputType::ERROR };
			}
			return {};
		}
	};
#endif

	const std::map<std::string, std::function<std::string>> SHADER_REPLACE = {
		{"", [&](std::string in) { return ""; }}, 		{"", [&](std::string in) { return ""; }}
	};

	template<class T>
	class Permute {

	private:
		std::vector<ShaderCodes> codes;
		nlohmann::json settings;
		GenerateOutput output;

	public:
		inline void preProcess(const std::map<std::string, std::function<std::string>> &callback) {
			for (auto& code : codes) {
				for (auto& codePart : code.code) {
					const auto eItr = end(codePart);
					auto startWordItr = eItr;
					auto endWordItr = eItr;
					auto paramStartItr = eItr;
					std::function<std::string> func(nullptr);
					for (auto itr = codePart.begin(); itr != eItr; itr++) {
						if (*itr == '§') {
							startWordItr = itr + 1;
							continue;
						}
						if (startWordItr != eItr && *itr == '_') {
							endWordItr = itr;
							const auto word = std::string(startWordItr, itr);
							const auto fncItr = callback.find(word);
							if (fncItr != end(callback)) {
								func = std::move(*fncItr);
								paramStartItr = itr + 1;
							}
							else {
								startWordItr = eItr;
							}
							continue;
						}
						if (startWordItr != eItr && *itr == ' ') {
							const auto replace = func(std::string(paramStartItr, itr));
							codePart.replace(startWordItr, endWordItr, replace);
						}
					}
				}
			}
		}

		inline bool generate(const std::vector<std::string>& dependencies = {}) {
			const GenerateInput input = { codes, dependencies, settings };
			output = T::generate(input);
			return success();
		}

		inline bool success() {
			return output.type != OutputType::ERROR;
		}

		inline std::string getContent() {
			return output.output;
		}

		NLOHMANN_DEFINE_TYPE_INTRUSIVE(Permute<T>, codes, settings)
	};

	template<class T>
	inline Permute<T> fromJson(nlohmann::json json) {
		return json.get<Permute<T>>();
	}


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
		} };
}
