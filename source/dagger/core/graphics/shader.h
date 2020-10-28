#pragma once

#include "core/core.h"
#include "core/bitenums.h"

#include <glad/glad.h>

enum class ShaderStage : unsigned
{
	None = 0,
	Vertex = 1,
	Fragment = 2,
	Geometry = 4,
	Compute = 8
};

ENABLE_BITMASK_OPERATORS(ShaderStage);

struct ShaderConfig
{
	String name{ "" };
	ShaderStage stages{ ShaderStage::None };
	Map<ShaderStage, String> paths{};
};

struct Shader
{
	enum class Uniforms
	{
		ProjectionMatrixId = 0,
		CameraViewMatrixId = 5,
		TextureBufferId = 10,
	};

	constexpr static UInt32 ms_ShaderStageCount = 4;

	inline static Map<ShaderStage, UInt32> s_ShaderStageHandles =
	{
		{ ShaderStage::Vertex, GL_VERTEX_SHADER },
		{ ShaderStage::Fragment, GL_FRAGMENT_SHADER },
		{ ShaderStage::Geometry, GL_GEOMETRY_SHADER },
		{ ShaderStage::Compute, GL_COMPUTE_SHADER }
	};

	inline static Map<String, ShaderStage> s_ShaderStageIndex =
	{
		{ "vertex-shader", ShaderStage::Vertex },
		{ "fragment-shader", ShaderStage::Fragment },
		{ "geometry-shader", ShaderStage::Geometry },
		{ "compute-shader", ShaderStage::Compute }
	};

	inline static Map<ShaderStage, String> s_ShaderStageNames =
	{
		{ ShaderStage::Vertex, "Vertex Shader" },
		{ ShaderStage::Fragment, "Fragment Shader" },
		{ ShaderStage::Geometry, "Geometry Shader" },
		{ ShaderStage::Compute, "Compute Shader" }
	};

	UInt32 programId;
	String shaderName;

	Shader() = default;

	Shader(ShaderConfig config_);
	~Shader();
};