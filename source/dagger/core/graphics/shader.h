#pragma once

#include "core/core.h"
#include "core/bitenums.h"

#include <glad/glad.h>

enum class EShaderStage : unsigned
{
	None = 0,
	Vertex = 1,
	Fragment = 2,
	Geometry = 4,
	Compute = 8
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
		CameraViewMatrixId = 5,
		TextureBufferId = 10,
	};

	constexpr static UInt32 ms_ShaderStageCount = 4;

	inline static Map<EShaderStage, UInt32> s_ShaderStageHandles =
	{
		{ EShaderStage::Vertex, GL_VERTEX_SHADER },
		{ EShaderStage::Fragment, GL_FRAGMENT_SHADER },
		{ EShaderStage::Geometry, GL_GEOMETRY_SHADER },
		{ EShaderStage::Compute, GL_COMPUTE_SHADER }
	};

	inline static Map<String, EShaderStage> s_ShaderStageIndex =
	{
		{ "vertex-shader", EShaderStage::Vertex },
		{ "fragment-shader", EShaderStage::Fragment },
		{ "geometry-shader", EShaderStage::Geometry },
		{ "compute-shader", EShaderStage::Compute }
	};

	inline static Map<EShaderStage, String> s_ShaderStageNames =
	{
		{ EShaderStage::Vertex, "Vertex Shader" },
		{ EShaderStage::Fragment, "Fragment Shader" },
		{ EShaderStage::Geometry, "Geometry Shader" },
		{ EShaderStage::Compute, "Compute Shader" }
	};

	UInt32 programId;
	String shaderName;

	Shader() = default;

	Shader(ShaderConfig config_);
	~Shader();
};