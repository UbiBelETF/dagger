#pragma once

#include "engine.h"
#include <spdlog/spdlog.h>

#include <SDL.h>

#define SDL_STBIMAGE_IMPLEMENTATION
#include "sdl_image.h"

#include "texture.h"

#include <deque>
#include <sstream>

using namespace dagger;

struct RenderSystem : public System
{
	struct SDLWindowDestroyer
	{
		void operator()(SDL_Window* window_) const
		{
			SDL_DestroyWindow(window_);
			SDL_Quit();
		}
	};

	std::unique_ptr<SDL_Window, SDLWindowDestroyer> m_Window;
	SDL_Renderer* m_Renderer;

	std::uint32_t m_WindowWidth;
	std::uint32_t m_WindowHeight;

	std::deque<LoadTextureRequest> m_TextureLoadingRequests;

	RenderSystem(std::uint32_t width_ = 640, std::uint32_t height_ = 480)
		: m_Window{ nullptr }
		, m_Renderer{ nullptr }
		, m_WindowWidth{ width_ }
		, m_WindowHeight{ height_ }
		, m_TextureLoadingRequests{}
	{}

	~RenderSystem() = default;
	RenderSystem(const RenderSystem&) = delete;

	void LoadTexture(LoadTextureRequest request_)
	{
		m_TextureLoadingRequests.push_back(std::move(request_));
	}

	void SpinUp(Engine& engine_) override
	{
		spdlog::info("Booting up renderer");
		auto& events = engine_.GetDispatcher();

		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			events.trigger<Error>(Error("SDL failed to initialize."));
			return;
		}

		m_Window.reset(SDL_CreateWindow("Dagger", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, m_WindowWidth, m_WindowHeight, SDL_WINDOW_SHOWN));

		if (m_Window == nullptr)
		{
			events.trigger<Error>(Error("Window creation failed."));
			return;
		}

		m_Renderer = SDL_CreateRenderer(m_Window.get(), -1, 0);
		if (m_Renderer == nullptr)
		{
			events.trigger<Error>(Error("Acquiring renderer failed."));
			return;
		}

		engine_.GetDispatcher().sink<LoadTextureRequest>().connect<&RenderSystem::LoadTexture>(this);
	}
			
	void Run(Engine& engine_) override
	{
		while (!m_TextureLoadingRequests.empty())
		{
			auto& request = m_TextureLoadingRequests.front();
			m_TextureLoadingRequests.pop_front();

			SDL_Surface* loadedSurface = STBIMG_Load(request.m_Path.c_str());
			if (loadedSurface == nullptr)
			{
				std::stringstream stream;
				stream << "Couldn't load surface from '" << request.m_Path << "'.";
				engine_.GetDispatcher().trigger<Error>(Error(stream.str()));
			}

			SDL_Texture* loadedTexture = SDL_CreateTextureFromSurface(m_Renderer, loadedSurface);
			if (loadedTexture == nullptr)
			{
				std::stringstream stream;
				stream << "Couldn't create texture for '" << request.m_Path << "'.";
				engine_.GetDispatcher().trigger<Error>(Error(stream.str()));
			}

			SDL_FreeSurface(loadedSurface);
			engine_.GetDispatcher().trigger<TextureLoaded>(request.Load(loadedTexture));
		}

		SDL_SetRenderDrawColor(m_Renderer, 0x64, 0x95, 0xED, 0xFF);
		SDL_RenderClear(m_Renderer);
		SDL_RenderPresent(m_Renderer);
	}

	void WindDown(Engine& engine_) override
	{
		engine_.GetDispatcher().sink<LoadTextureRequest>().disconnect<&RenderSystem::LoadTexture>(this);

		spdlog::info("Winding down renderer");

		m_Window.reset();
		m_Renderer = nullptr;
	}
};

struct RenderComponent
{
	SDL_Texture* m_Texture;
};