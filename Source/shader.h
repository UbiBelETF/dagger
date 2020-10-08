#pragma once

#include <string>
#include "bitenums.h"

#include <glad.h>

namespace dagger
{
	enum class ShaderStage : unsigned 
	{
		Vertex = 1,
		Fragment = 2,
		Geometry = 4,
		Compute = 8
	};

	ENABLE_BITMASK_OPERATORS(ShaderStage);

	struct Shader
	{
		constexpr static unsigned int ms_ShaderStageCount = 4;

		constexpr static unsigned int ms_ShaderStageHandles[ms_ShaderStageCount] =
		{
			GL_VERTEX_SHADER,
			GL_FRAGMENT_SHADER,
			GL_GEOMETRY_SHADER,
			GL_COMPUTE_SHADER
		};

		const static inline char* ms_ShaderStageNames[ms_ShaderStageCount] =
		{
			"Vertex Shader",
			"Fragment Shader",
			"Geometry Shader",
			"Compute Shader"
		};

		const static inline char* ms_ShaderStageExt[ms_ShaderStageCount] =
		{
			".vs.glsl",
			".fs.glsl",
			".gs.glsl",
			".cs.glsl"
		};

		unsigned int m_ProgramId;
		std::string m_ShaderName;

		Shader(std::string name_, ShaderStage stages_);
		~Shader();
	};
}