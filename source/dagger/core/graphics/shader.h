#pragma once

#include "core/core.h"
#include "core/bitenums.h"

#include <glad/glad.h>

enum class EShaderStage : unsigned
{
	None = 0,
	Vertex = 1,
	Fragment = 2,
};

ENABLE_BITMASK_OPERATORS(EShaderStage);

struct ShaderConfig
{
	String name{ "" };
	EShaderStage stages{ EShaderStage::None };
	Map<EShaderStage, String> paths{};
};

struct Shader
{
	enum class EUniforms
	{
		ProjectionMatrixId = 0,
		CameraViewMatrixId = 1,
	};

	constexpr static UInt32 ms_ShaderStageCount = 4;

	inline static Map<EShaderStage, UInt32> s_ShaderStageHandles =
	{
		{ EShaderStage::Vertex, GL_VERTEX_SHADER },
		{ EShaderStage::Fragment, GL_FRAGMENT_SHADER },
	};

	inline static Map<String, EShaderStage> s_ShaderStageIndex =
	{
		{ "vertex-shader", EShaderStage::Vertex },
		{ "fragment-shader", EShaderStage::Fragment },
	};

	inline static Map<EShaderStage, String> s_ShaderStageNames =
	{
		{ EShaderStage::Vertex, "Vertex Shader" },
		{ EShaderStage::Fragment, "Fragment Shader" },
	};

	UInt32 programId;
	String shaderName;

	Shader() = default;

	Shader(ShaderConfig config_);
	~Shader();
};