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

	class ShaderTraverser : public glslang::TIntermTraverser {
	public:
		virtual void postProcess() {}
		virtual bool isValid(const GlslSettings& settings) { return true; };
	};

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
	static const TBuiltInResource DefaultTBuiltInResource = InitResources();

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

	
	struct NoCache {
		SPR_NODISCARD inline std::optional<ResultOrError> get(const std::string& dependencies) const {
			return std::optional<ResultOrError>();
		}

		SPR_NODISCARD inline void add(const std::string& dependencies, const ResultOrError& result) {}
	};
	
	struct AllCache {

		std::unordered_map<std::string, ResultOrError> cache;

		SPR_NODISCARD inline std::optional<ResultOrError> get(const std::string& dependencies) const {
			auto iterator = cache.find(dependencies);
			if(iterator != cache.end()) {
				return std::optional<ResultOrError>(iterator->second);
			}
			return std::optional<ResultOrError>();
		}

		SPR_NODISCARD inline void add(const std::string& dependencies, const ResultOrError& result) {
			cache.emplace(dependencies, result);
		}
	};

	template<typename T>
	concept CacheConcept = requires(const T& cache, T & cacheNoConst, const std::string& dependencies, const ResultOrError& result) {
		{ cache.get(dependencies) } -> std::same_as<std::optional<ResultOrError>>;
		{ cacheNoConst.add(dependencies, result) };
	};
	static_assert(CacheConcept<NoCache>, "NoChache must implement CacheConcept");
	static_assert(CacheConcept<AllCache>, "AllChache must implement CacheConcept");

	template<CacheConcept T = NoCache>
	class PermuteGLSL {
		GlslSettings settings;
		std::vector<const char*> input;
		mutable T cache{};

	public:
		using CacheType = T;
		std::vector<ShaderTraverser*> traverser;

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
			const auto cachedValue = cache.get(preamble);
			if(cachedValue) {
				return *cachedValue;
			}
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
				node->traverse(travPtr);
				travPtr->postProcess();
			}
			std::vector<unsigned int> outputData;
			glslang::GlslangToSpv(*interm, outputData);
			ResultOrError result{ std::move(outputData) };
			cache.add(preamble, result);
			return result;
		}
	};

	inline EShLanguage getLanguageFromExtension(const std::string& ext) {
		if (ext == ".vert" || ext == ".vsh" || ext == ".vs")
			return EShLangVertex;
		else if (ext == ".tesc" || ext == ".tcs")
			return EShLangTessControl;
		else if (ext == ".tese" || ext == ".tes")
			return EShLangTessEvaluation;
		else if (ext == ".geom" || ext == ".gsh")
			return EShLangGeometry;
		else if (ext == ".frag" || ext == ".fsh" || ext == ".fs")
			return EShLangFragment;
		else if (ext == ".comp" || ext == ".csh")
			return EShLangCompute;
		else if (ext == ".rgen")
			return EShLangRayGen;
		else if (ext == ".rint")
			return EShLangIntersect;
		else if (ext == ".rahit")
			return EShLangAnyHit;
		else if (ext == ".rchit")
			return EShLangClosestHit;
		else if (ext == ".rmiss")
			return EShLangMiss;
		else if (ext == ".rcall")
			return EShLangCallable;
		else if (ext == ".tasknv")
			return EShLangTaskNV;
		else if (ext == ".meshnv")
			return EShLangMeshNV;
		throw std::runtime_error(
			"Unknown shader extension: " + ext +
			". Please use one of the following: .vert, .tesc, .tese, .geom, .frag, .comp, .rgen, .rint, .rahit, .rchit, .rmiss, .rcall, .tasknv or .meshnv");
	}

	template<CacheConcept T = NoCache>
	struct Permute {
		GlslSettings settings = {};
		std::vector<ShaderTraverser*> traverser;
	private:
		std::unordered_map<std::string, std::vector<std::string>> inputMap;
		std::unordered_map<std::string, PermuteGLSL<T>*> cache;
	public:

		Permute() {
			glslang::InitializeProcess();
		}

		~Permute()
		{
			for (auto &[l1, l2] : cache)
			{
				delete l2;
			}
			glslang::FinalizeProcess();
		}

		SPR_NODISCARD inline PermuteGLSL<T>* getGLSLPermute(const std::string& name) {
			auto cacheItr = cache.find(name);
			if(cacheItr != cache.end()) {
				return cacheItr->second;
			}
			auto iterator = inputMap.find(name);
			if(iterator == inputMap.end()) {
				return nullptr;
			}
			const auto& stringValues = iterator->second;
			std::vector<const char*> inputs(stringValues.size());
			std::transform(stringValues.begin(), stringValues.end(),
				inputs.begin(), [](const std::string& str) { return str.c_str(); });
			GlslSettings settings = this->settings;
			settings.shaderType = getLanguageFromExtension(
				name.substr(name.find_last_of('.')));
			auto glsl = new PermuteGLSL<T>(settings, std::move(inputs));
			glsl->traverser = traverser;
			cache.emplace(name, glsl);
			return glsl;
		}

		SPR_NODISCARD inline PermuteGLSL<T>* fromStrings(const std::string& name, std::vector<std::string>&& input) {
#ifndef NDEBUG
			const auto value = inputMap.find(name);
			if(value != inputMap.end()) {
				std::stringstream ss;
				ss << "ShaderPermute: Shader with name '" << name
				   << "' already exists. Please use a different name.";
				throw std::runtime_error(ss.str());
			}
#endif // !NDEBUG
			inputMap[name] = std::move(input);
			return getGLSLPermute(name);
		}

#ifndef SPR_NO_FSTREAM
		SPR_NODISCARD inline PermuteGLSL<T>* fromFile(const std::string& path) {
			std::ifstream inputfile(path);
			if (!inputfile)
				throw std::runtime_error("File not found!");
			std::vector<std::string> inputs;
			inputs.reserve(128);
			for (std::string input; std::getline(inputfile, input);) {
				input.append("\n");
				inputs.push_back(input);
			}
			return fromStrings(path, std::move(inputs));
		}
#endif
	};

} // namespace permute
