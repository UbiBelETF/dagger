#include "shader.h"
#include "core/files.h"
#include "core/engine.h"

#include <cmath>

using namespace dagger;

Shader::Shader(ShaderConfig config_)
	: programId{ 0 }
	, shaderName{ config_.name }
{
	Logger::info("Constructing shader program '{}'", config_.name);

	constexpr auto count = s_ShaderStageCount + 1;
	Sequence<UInt32> shaderIds;

	programId = glCreateProgram();

	for (auto [stage, path] : config_.paths)
	{
		Logger::info("Loading {}'s {}...", config_.name, Shader::s_ShaderStageNames[stage]);
		UInt32 id = glCreateShader(Shader::s_ShaderStageHandles[stage]);

		Logger::trace(" path: {}", path);
		String source = ReadFromFile(path);

		if (source.empty())
		{
			Engine::Dispatcher().trigger<Error>(Error{ fmt::format("Shader path empty or not found: {}", path) });
			return;
		}

		{
			auto sourceCode = source.c_str();
			glShaderSource(id, 1, &sourceCode, nullptr);
			glCompileShader(id);
			{
				GLint success;
				GLchar infoLog[512];
				glGetShaderiv(id, GL_COMPILE_STATUS, &success);
				if (!success)
				{
					glGetShaderInfoLog(id, 512, nullptr, infoLog);
					auto errorMessage = fmt::format("Shader error ({}): {}", config_.name, infoLog);
					Logger::error(errorMessage);
					Engine::Dispatcher().trigger<Error>(Error{ errorMessage });
					return;
				}
			}

			glAttachShader(programId, id);
			shaderIds.push_back(id);
			Logger::info("{} successfully compiled and attached to program '{}'", Shader::s_ShaderStageNames[stage], config_.name);
		}
	}

	glLinkProgram(programId);

	{
		GLint success;
		GLchar infoLog[512];
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programId, 512, NULL, infoLog);
			auto errorMessage = fmt::format("Shader linking error ({}): {}", config_.name, infoLog);
			Logger::error(errorMessage);
			Engine::Dispatcher().trigger<Error>(Error{ errorMessage });
			return;
		}
	}

	Logger::info("Successfully linked shader '{}'", config_.name);

	for (UInt32 id : shaderIds)
	{
		glDeleteShader(id);
	}

	programId = programId;
	assert(programId != 0);

	if (!s_FirstLoadedShader)
	{
		s_FirstLoadedShader.reset(this);
	}
}

Shader::~Shader()
{
	if (programId != 0)
		glDeleteProgram(programId);

	Logger::info("Successfully destroyed shader '{}'", shaderName);
}