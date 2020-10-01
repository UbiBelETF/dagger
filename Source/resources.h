#pragma once

#include "engine.h"
#include "events.h"
#include "texture.h"

#include <spdlog/spdlog.h>

#include <string>
#include <fstream>
#include <filesystem>

using namespace dagger;

struct ResourceSystem : public System
{
	std::string m_Path;

	ResourceSystem(std::string path_)
		: m_Path{ path_ }
	{}

	void SpinUp(dagger::Engine& engine_) override
	{
		std::filesystem::path path{ std::filesystem::absolute(std::filesystem::path{ m_Path.c_str() }) };
		spdlog::info("Loading resources from '{}'.", path.string().c_str());

		std::ifstream infile;
		infile.open(path);

		if (!infile.is_open())
		{
			engine_.GetDispatcher().trigger<Error>(Error("Failed to open resource configuration."));
			return;
		}

		std::string line;

		while (infile >> line)
		{
			spdlog::info("Reading from file and sending to texture loader: {}", line);
			engine_.GetDispatcher().trigger<LoadTextureRequest>(LoadTextureRequest{ line });
		}

		spdlog::info("Resources loaded.");
	}

	void Run(dagger::Engine& engine_) override
	{
	}

	void WindDown(dagger::Engine& engine_) override
	{
	}
};

