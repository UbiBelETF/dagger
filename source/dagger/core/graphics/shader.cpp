#include "shader.h"
#include "core/files.h"
#include "core/engine.h"

#include <spdlog/spdlog.h>
#include <fmt/format.h>
#include <cmath>

using namespace dagger;

Shader::Shader(String name_, ShaderStage stages_)
	: m_ProgramId{ 0 }
	, m_ShaderName{ name_ }
{
	Logger::info("Constructing shader program '{}'", name_);
	
	constexpr auto count = ms_ShaderStageCount + 1;
	UInt32 shaderIds[count] = { 0, };

	for (UInt32 i = 0; i < count; ++i)
		shaderIds[i] = 0;

	shaderIds[0] = glCreateProgram();

	for (UInt32 i = 1; i < count; ++i)
	{
		UInt32 value = (UInt32) std::pow(2, i - 1);
		if (((UInt32)stages_ & value) == value)
		{
			Logger::info("Loading {}'s {}...", name_, ms_ShaderStageNames[i - 1]);
			shaderIds[i] = glCreateShader(ms_ShaderStageHandles[i - 1]);

			auto path = fmt::format("shaders/{}{}", name_, ms_ShaderStageExt[i - 1]);
			Logger::trace(" path: {}", path);
			String source = ReadFromFile(path);

			if (source.empty())
			{
				Engine::Dispatcher().trigger<Error>(Error(fmt::format("Shader path empty or not found: {}", path)));
				return;
			}

			{
				auto sourceCode = source.c_str();
				glShaderSource(shaderIds[i], 1, &sourceCode, nullptr);
				glCompileShader(shaderIds[i]);
				{
					GLint success; 
					GLchar infoLog[512]; 
					glGetShaderiv(shaderIds[i], GL_COMPILE_STATUS, &success); 
					if (!success)
					{
						glGetShaderInfoLog(shaderIds[i], 512, nullptr, infoLog);
						auto errorMessage = fmt::format("Shader error ({}): {}", name_, infoLog);
						Logger::error(errorMessage);
						Engine::Dispatcher().trigger<Error>(Error(errorMessage));
						return;
					}
				}

				glAttachShader(shaderIds[0], shaderIds[i]);
				Logger::info("{} successfully compiled and attached to program '{}'", ms_ShaderStageNames[i - 1], name_);
			}
		}
	}

	glLinkProgram(shaderIds[0]);

	{
		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(shaderIds[0], GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderIds[0], 512, NULL, infoLog);
			auto errorMessage = fmt::format("Shader linking error ({}): {}", name_, infoLog);
			Logger::error(errorMessage);
			Engine::Dispatcher().trigger<Error>(Error(errorMessage));
			return;
		}
	}

	Logger::info("Successfully linked shader '{}'", name_);

	for (UInt32 i = 1; i < count; ++i)
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

	Logger::info("Successfully destroyed shader '{}'", m_ShaderName);
}
