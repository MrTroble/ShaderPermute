/*
   Copyright 2021 MrTroble

   Licensed under the Apache License,
   Version 2.0(the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

	   http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
#pragma once

#define SPR_VERSION_MAJOR 2
#define SPR_VERSION_MINOR 0
#define SPR_VERSION_PATCH 0

#ifdef SPR_USE_FORMAT_LIB
#include <format>
#else
#include <sstream>
#endif // SPR_USE_FORMAT_LIB

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

#if defined(_NODISCARD) && !(defined(SPR_DISABLE_NODISCARD))
#define SPR_NODISCARD _NODISCARD
#elif !(defined(SPR_NODISCARD))
#define SPR_NODISCARD
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
#include <glslang/MachineIndependent/localintermediate.h>
#endif

#ifndef SPR_NO_STATIC
#define SPR_STATIC static
#endif

NLOHMANN_JSON_SERIALIZE_ENUM(EShLanguage,
	{ {EShLangVertex, "vertex"},
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
	NLOHMANN_JSON_SERIALIZE_ENUM(EShClient, { {EShClientNone, "none"},
											 {EShClientVulkan, "vulkan"},
											 {EShClientOpenGL, "opengl"} });
	NLOHMANN_JSON_SERIALIZE_ENUM(EShTargetClientVersion,
		{ {EShTargetVulkan_1_0, "vulkan_1_0"},
		 {EShTargetVulkan_1_1, "vulkan_1_1"},
		 {EShTargetVulkan_1_2, "vulkan_1_2"},
		 {EShTargetOpenGL_450, "opengl_450"} });
	NLOHMANN_JSON_SERIALIZE_ENUM(EShTargetLanguage,
		{ {EShTargetNone, "none"}, {EShTargetSpv, "spv"} });
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

namespace permute {

	using lookup =
		std::map<std::string, std::function<std::string(const std::string&)>>;

	enum class ShaderCodeFlags { NONE = 0, REQUIRED = 1 };

	NLOHMANN_JSON_SERIALIZE_ENUM(ShaderCodeFlags,
		{ {ShaderCodeFlags::NONE, "none"},
		 {ShaderCodeFlags::REQUIRED, "required"} })

		SPR_NODISCARD inline bool isRequired(const ShaderCodeFlags flag) {
		return (int)flag & (int)ShaderCodeFlags::REQUIRED;
	}

	struct GlslSettings {
		EShLanguage shaderType;
		glslang::EShClient targetClient = glslang::EShClient::EShClientVulkan;
		glslang::EShTargetClientVersion targetVersion =
			glslang::EShTargetClientVersion::EShTargetVulkan_1_0;
		glslang::EShTargetLanguage targetLanguage =
			glslang::EShTargetLanguage::EShTargetSpv;
		glslang::EShTargetLanguageVersion targetLanguageVersion =
			glslang::EShTargetLanguageVersion::EShTargetSpv_1_0;

		friend void to_json(nlohmann::json& nlohmann_json_j,
			const GlslSettings& nlohmann_json_t) {
			NLOHMANN_JSON_TO(shaderType);
			SPR_OPTIONAL_TO(targetClient);
			SPR_OPTIONAL_TO(targetVersion);
			SPR_OPTIONAL_TO(targetLanguage);
			SPR_OPTIONAL_TO(targetLanguageVersion);
		}

		friend void from_json(const nlohmann::json& nlohmann_json_j,
			GlslSettings& nlohmann_json_t) {
			NLOHMANN_JSON_FROM(shaderType);
			SPR_OPTIONAL_FROM(targetClient);
			SPR_OPTIONAL_FROM(targetVersion);
			SPR_OPTIONAL_FROM(targetLanguage);
			SPR_OPTIONAL_FROM(targetLanguageVersion);
		}
	};

	class ShaderTraverser;
	static std::vector<permute::ShaderTraverser*> traverser;

	class ShaderTraverser {
	public:
		ShaderTraverser() { permute::traverser.push_back(this); }

		~ShaderTraverser() {
			permute::traverser.erase(
				std::remove(begin(permute::traverser), end(permute::traverser), this));
		}

		virtual void visitSymbol(glslang::TIntermSymbol*) {}
		virtual void visitConstantUnion(glslang::TIntermConstantUnion*) {}
		virtual bool visitBinary(glslang::TVisit, glslang::TIntermBinary*) {
			return true;
		}
		virtual bool visitUnary(glslang::TVisit, glslang::TIntermUnary*) {
			return true;
		}
		virtual bool visitSelection(glslang::TVisit, glslang::TIntermSelection*) {
			return true;
		}
		virtual bool visitAggregate(glslang::TVisit, glslang::TIntermAggregate*) {
			return true;
		}
		virtual bool visitLoop(glslang::TVisit, glslang::TIntermLoop*) {
			return true;
		}
		virtual bool visitBranch(glslang::TVisit, glslang::TIntermBranch*) {
			return true;
		}
		virtual bool visitSwitch(glslang::TVisit, glslang::TIntermSwitch*) {
			return true;
		}
		virtual void postProcess() {}
		virtual bool isValid(const GlslSettings& settings) = 0;
	};

	namespace impl {

		class ShaderTraverser : public glslang::TIntermTraverser {
		public:
			permute::ShaderTraverser* traverser;

			ShaderTraverser(permute::ShaderTraverser* traverser) : traverser(traverser) {}

			virtual void visitSymbol(glslang::TIntermSymbol* s) {
				traverser->visitSymbol(s);
			}

			virtual void visitConstantUnion(glslang::TIntermConstantUnion* s) {
				traverser->visitConstantUnion(s);
			}

			virtual bool visitBinary(glslang::TVisit v, glslang::TIntermBinary* s) {
				return traverser->visitBinary(v, s);
			}
			virtual bool visitUnary(glslang::TVisit v, glslang::TIntermUnary* s) {
				return traverser->visitUnary(v, s);
			}
			virtual bool visitSelection(glslang::TVisit v, glslang::TIntermSelection* s) {
				return traverser->visitSelection(v, s);
			}
			virtual bool visitAggregate(glslang::TVisit v, glslang::TIntermAggregate* s) {
				return traverser->visitAggregate(v, s);
			}
			virtual bool visitLoop(glslang::TVisit v, glslang::TIntermLoop* s) {
				return traverser->visitLoop(v, s);
			}
			virtual bool visitBranch(glslang::TVisit v, glslang::TIntermBranch* s) {
				return traverser->visitBranch(v, s);
			}
			virtual bool visitSwitch(glslang::TVisit v, glslang::TIntermSwitch* s) {
				return traverser->visitSwitch(v, s);
			}
		};

	} // namespace impl

	inline TBuiltInResource InitResources()
	{
		TBuiltInResource Resources{};
		Resources.maxLights = 32;
		Resources.maxClipPlanes = 6;
		Resources.maxTextureUnits = 32;
		Resources.maxTextureCoords = 32;
		Resources.maxVertexAttribs = 64;
		Resources.maxVertexUniformComponents = 4096;
		Resources.maxVaryingFloats = 64;
		Resources.maxVertexTextureImageUnits = 32;
		Resources.maxCombinedTextureImageUnits = 80;
		Resources.maxTextureImageUnits = 32;
		Resources.maxFragmentUniformComponents = 4096;
		Resources.maxDrawBuffers = 32;
		Resources.maxVertexUniformVectors = 128;
		Resources.maxVaryingVectors = 8;
		Resources.maxFragmentUniformVectors = 16;
		Resources.maxVertexOutputVectors = 16;
		Resources.maxFragmentInputVectors = 15;
		Resources.minProgramTexelOffset = -8;
		Resources.maxProgramTexelOffset = 7;
		Resources.maxClipDistances = 8;
		Resources.maxComputeWorkGroupCountX = 65535;
		Resources.maxComputeWorkGroupCountY = 65535;
		Resources.maxComputeWorkGroupCountZ = 65535;
		Resources.maxComputeWorkGroupSizeX = 1024;
		Resources.maxComputeWorkGroupSizeY = 1024;
		Resources.maxComputeWorkGroupSizeZ = 64;
		Resources.maxComputeUniformComponents = 1024;
		Resources.maxComputeTextureImageUnits = 16;
		Resources.maxComputeImageUniforms = 8;
		Resources.maxComputeAtomicCounters = 8;
		Resources.maxComputeAtomicCounterBuffers = 1;
		Resources.maxVaryingComponents = 60;
		Resources.maxVertexOutputComponents = 64;
		Resources.maxGeometryInputComponents = 64;
		Resources.maxGeometryOutputComponents = 128;
		Resources.maxFragmentInputComponents = 128;
		Resources.maxImageUnits = 8;
		Resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
		Resources.maxCombinedShaderOutputResources = 8;
		Resources.maxImageSamples = 0;
		Resources.maxVertexImageUniforms = 0;
		Resources.maxTessControlImageUniforms = 0;
		Resources.maxTessEvaluationImageUniforms = 0;
		Resources.maxGeometryImageUniforms = 0;
		Resources.maxFragmentImageUniforms = 8;
		Resources.maxCombinedImageUniforms = 8;
		Resources.maxGeometryTextureImageUnits = 16;
		Resources.maxGeometryOutputVertices = 256;
		Resources.maxGeometryTotalOutputComponents = 1024;
		Resources.maxGeometryUniformComponents = 1024;
		Resources.maxGeometryVaryingComponents = 64;
		Resources.maxTessControlInputComponents = 128;
		Resources.maxTessControlOutputComponents = 128;
		Resources.maxTessControlTextureImageUnits = 16;
		Resources.maxTessControlUniformComponents = 1024;
		Resources.maxTessControlTotalOutputComponents = 4096;
		Resources.maxTessEvaluationInputComponents = 128;
		Resources.maxTessEvaluationOutputComponents = 128;
		Resources.maxTessEvaluationTextureImageUnits = 16;
		Resources.maxTessEvaluationUniformComponents = 1024;
		Resources.maxTessPatchComponents = 120;
		Resources.maxPatchVertices = 32;
		Resources.maxTessGenLevel = 64;
		Resources.maxViewports = 16;
		Resources.maxVertexAtomicCounters = 0;
		Resources.maxTessControlAtomicCounters = 0;
		Resources.maxTessEvaluationAtomicCounters = 0;
		Resources.maxGeometryAtomicCounters = 0;
		Resources.maxFragmentAtomicCounters = 8;
		Resources.maxCombinedAtomicCounters = 8;
		Resources.maxAtomicCounterBindings = 1;
		Resources.maxVertexAtomicCounterBuffers = 0;
		Resources.maxTessControlAtomicCounterBuffers = 0;
		Resources.maxTessEvaluationAtomicCounterBuffers = 0;
		Resources.maxGeometryAtomicCounterBuffers = 0;
		Resources.maxFragmentAtomicCounterBuffers = 1;
		Resources.maxCombinedAtomicCounterBuffers = 1;
		Resources.maxAtomicCounterBufferSize = 16384;
		Resources.maxTransformFeedbackBuffers = 4;
		Resources.maxTransformFeedbackInterleavedComponents = 64;
		Resources.maxCullDistances = 8;
		Resources.maxCombinedClipAndCullDistances = 8;
		Resources.maxSamples = 4;
		Resources.maxMeshOutputVerticesNV = 256;
		Resources.maxMeshOutputPrimitivesNV = 512;
		Resources.maxMeshWorkGroupSizeX_NV = 32;
		Resources.maxMeshWorkGroupSizeY_NV = 1;
		Resources.maxMeshWorkGroupSizeZ_NV = 1;
		Resources.maxTaskWorkGroupSizeX_NV = 32;
		Resources.maxTaskWorkGroupSizeY_NV = 1;
		Resources.maxTaskWorkGroupSizeZ_NV = 1;
		Resources.maxMeshViewCountNV = 4;

		Resources.limits.nonInductiveForLoops = 1;
		Resources.limits.whileLoops = 1;
		Resources.limits.doWhileLoops = 1;
		Resources.limits.generalUniformIndexing = 1;
		Resources.limits.generalAttributeMatrixVectorIndexing = 1;
		Resources.limits.generalVaryingIndexing = 1;
		Resources.limits.generalSamplerIndexing = 1;
		Resources.limits.generalVariableIndexing = 1;
		Resources.limits.generalConstantMatrixVectorIndexing = 1;

		return Resources;
	}
	const TBuiltInResource DefaultTBuiltInResource = InitResources();


	struct Dependency {
		std::string name;
		std::string value = "1";
	};

	enum ResultType
	{
		SUCCESS,
		COMPILE_ERROR
	};

	struct ResultOrError {
		ResultType type;
		std::vector<unsigned int> output;
		std::string error;

		ResultOrError(std::vector<unsigned int>&& output, ResultType type = ResultType::SUCCESS)
			: type(type), output(std::move(output)) {
		}

		ResultOrError(const std::string& error, ResultType type = ResultType::COMPILE_ERROR)
			: error(error), type(type) {
		}

		operator bool() const {
			return type == ResultType::SUCCESS;
		}

		bool operator!() const {
			return type != ResultType::SUCCESS;
		}
	};

	class PermuteGLSL {
		GlslSettings settings;
		std::vector<const char*> input;
	public:
		PermuteGLSL(GlslSettings settings, const std::vector<const char*>& input)
			: settings(settings), input(input) {
		}

		PermuteGLSL(GlslSettings settings, std::vector<const char*>&& input)
			: settings(settings), input(std::move(input)) {
		}

		SPR_NODISCARD inline ResultOrError
			generate(const std::vector<Dependency>& dependcies = {}) const {
			std::stringstream inputDataDefsStream;
			for (const auto& dep : dependcies)
			{
				inputDataDefsStream << "#define " << dep.name << " " << dep.value << "\n";
			}
			auto preamble = inputDataDefsStream.str();
			auto shader = new glslang::TShader(settings.shaderType);
			shader->setPreamble(preamble.c_str());
			shader->setStrings(input.data(), static_cast<int>(input.size()));
			shader->setEnvInput(glslang::EShSourceGlsl, settings.shaderType,
				settings.targetClient, 100);
			shader->setEnvClient(settings.targetClient, settings.targetVersion);
			shader->setEnvTarget(settings.targetLanguage,
				settings.targetLanguageVersion);
			if (!shader->parse(&DefaultTBuiltInResource, 460, EProfile::ECoreProfile, false, true,
				EShMessages::EShMsgVulkanRules)) {
				return ResultOrError{shader->getInfoLog()};
			}
			const auto interm = shader->getIntermediate();
			const auto node = interm->getTreeRoot();
			for (const auto travPtr : traverser) {
				if (!travPtr->isValid(settings))
					continue;
				impl::ShaderTraverser trav(travPtr);
				node->traverse(&trav);
				travPtr->postProcess();
			}
			std::vector<unsigned int> outputData;
			glslang::GlslangToSpv(*interm, outputData);
			return ResultOrError{ std::move(outputData) };

		}
	};

	struct Permute {
		GlslSettings settings = {};
	    std::unordered_map<std::string, std::vector<std::string>> inputMap;

		Permute() {
			glslang::InitializeProcess();
		}

		~Permute()
		{
			glslang::FinalizeProcess();
		}

		inline PermuteGLSL getGLSLPermute(const std::string& name) const {
			auto iterator = inputMap.find(name);
#ifndef NDEBUG
			if(iterator == inputMap.end()) {
				std::stringstream ss;
				ss << "ShaderPermute: No shader found with name '" << name << "'";
				throw std::runtime_error(ss.str());
			}
#endif // !NDEBUG
			const auto& stringValues = iterator->second;
			std::vector<const char*> inputs(stringValues.size());
			std::transform(stringValues.begin(), stringValues.end(),
				inputs.begin(), [](const std::string& str) { return str.c_str(); });
			return PermuteGLSL(settings, std::move(inputs));
		}

#ifndef SPR_NO_FSTREAM
		inline PermuteGLSL fromFile(const std::string& path) {
			std::ifstream inputfile(path);
			if (!inputfile)
				throw std::runtime_error("File not found!");
			std::vector<std::string> inputs;
			inputs.reserve(128);
			for (std::string input; std::getline(inputfile, input);) {
				input.append("\n");
				inputs.push_back(input);
			}
			inputMap[path] = std::move(inputs);
			return getGLSLPermute(path);
		}
#endif
	};

} // namespace permute
