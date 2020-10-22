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

struct Shader
{
	enum class Uniforms
	{
		ProjectionMatrixId = 0,
		TextureBufferId = 5,
	};

	constexpr static UInt32 ms_ShaderStageCount = 4;

	constexpr static UInt32 ms_ShaderStageHandles[ms_ShaderStageCount] =
	{
		GL_VERTEX_SHADER,
		GL_FRAGMENT_SHADER,
		GL_GEOMETRY_SHADER,
		GL_COMPUTE_SHADER
	};

	const static inline Char* ms_ShaderStageNames[ms_ShaderStageCount] =
	{
		"Vertex Shader",
		"Fragment Shader",
		"Geometry Shader",
		"Compute Shader"
	};

	const static inline Char* ms_ShaderStageExt[ms_ShaderStageCount] =
	{
		".vs.glsl",
		".fs.glsl",
		".gs.glsl",
		".cs.glsl"
	};

	UInt32 m_ProgramId;
	String m_ShaderName;

    Shader() = default;

	Shader(String name_, ShaderStage stages_);
	~Shader();
};
