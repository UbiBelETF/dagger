#include "shader.h"
#include "files.h"
#include "engine.h"

#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <cmath>

using namespace dagger;

Shader::Shader(std::string name_, ShaderStage stages_)
	: m_ProgramId{ 0 }
	, m_ShaderName{ name_ }
{
	spdlog::info("Constructing shader program '{}'", name_);
	
	constexpr auto count = ms_ShaderStageCount + 1;
	unsigned int shaderIds[count] = { 0, };

	for (int i = 0; i < count; ++i)
		shaderIds[i] = 0;

	shaderIds[0] = glCreateProgram();

	for (int i = 1; i < count; ++i)
	{
		unsigned int value = std::pow(2, i - 1);
		if (((unsigned int)stages_ & value) == value)
		{
			spdlog::info("Loading {}'s {}...", name_, ms_ShaderStageNames[i - 1]);
			shaderIds[i] = glCreateShader(ms_ShaderStageHandles[i - 1]);

			auto path = fmt::format("Res/Shaders/{}{}", name_, ms_ShaderStageExt[i - 1]);
			spdlog::trace(" path: {}", path);
			std::string source = ReadFromFile(path);

			if (source.empty())
			{
				Engine::Dispatch().trigger<Error>(Error(fmt::format("Shader path empty or not found: {}", path)));
				return;
			}

			{
				auto sourceCode = source.c_str();
				glShaderSource(shaderIds[i], 1, &sourceCode, nullptr);
				glCompileShader(shaderIds[i]);
				{
					int  success; 
					char infoLog[512]; 
					glGetShaderiv(shaderIds[i], GL_COMPILE_STATUS, &success); 
					if (!success)
					{
						glGetShaderInfoLog(shaderIds[i], 512, nullptr, infoLog);
						auto errorMessage = fmt::format("Shader error ({}): {}", name_, infoLog);
						spdlog::error(errorMessage);
						Engine::Dispatch().trigger<Error>(Error(errorMessage));
						return;
					}
				}

				glAttachShader(shaderIds[0], shaderIds[i]);
				spdlog::info("{} successfully compiled and attached to program '{}'", ms_ShaderStageNames[i - 1], name_);
			}
		}
	}

	glLinkProgram(shaderIds[0]);

	{
		int  success;
		char infoLog[512];
		glGetProgramiv(shaderIds[0], GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderIds[0], 512, NULL, infoLog);
			auto errorMessage = fmt::format("Shader linking error ({}): {}", name_, infoLog);
			spdlog::error(errorMessage);
			Engine::Dispatch().trigger<Error>(Error(errorMessage));
			return;
		}
	}

	spdlog::info("Successfully linked shader '{}'", name_);

	for (int i = 1; i < count; ++i)
	{
		if(shaderIds[i] != 0)
			glDeleteShader(shaderIds[i]);
	}

	m_ProgramId = shaderIds[0];
	assert(m_ProgramId != 0);
}

Shader::~Shader()
{
	if(m_ProgramId != 0)
		glDeleteProgram(m_ProgramId);

	spdlog::info("Successfully destroyed shader '{}'", m_ShaderName);
}
