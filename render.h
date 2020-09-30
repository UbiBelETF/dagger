#pragma once

#include "engine.h"
#include <spdlog/spdlog.h>

#include <SDL.h>

namespace dagger
{
	struct RenderSystem : public dagger::System
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

		RenderSystem(std::uint32_t width_ = 640, std::uint32_t height_ = 480)
			: m_Window{ nullptr }
			, m_Renderer{ nullptr }
			, m_WindowWidth{ width_ }
			, m_WindowHeight{ height_ }
		{}

		~RenderSystem() = default;
		RenderSystem(const RenderSystem&) = delete;

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
		}
			
		void Run(Engine&) override
		{
			SDL_SetRenderDrawColor(m_Renderer, 0x64, 0x95, 0xED, 0xFF);
			SDL_RenderClear(m_Renderer);
			SDL_RenderPresent(m_Renderer);
		}

		void WindDown(Engine&) override
		{
			spdlog::info("Winding down renderer");

			m_Window.reset();
			m_Renderer = nullptr;
		}
	};
}