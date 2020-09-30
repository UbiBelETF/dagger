#pragma once

#include "system.h"
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
		SDL_Surface* m_Surface;

		RenderSystem()
			: m_Window { nullptr }
			, m_Surface{ nullptr }
		{}

		~RenderSystem() = default;
		RenderSystem(const RenderSystem&) = delete;

		void SpinUp(entt::dispatcher& events_) override
		{
			spdlog::info("Booting up renderer");

			if (SDL_Init(SDL_INIT_VIDEO) < 0)
			{
				events_.trigger<Error>(Error("SDL failed to initialize."));
				return;
			}

			m_Window.reset(SDL_CreateWindow("Dagger", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN));

			if (m_Window == nullptr)
			{
				events_.trigger<Error>(Error("Window creation failed."));
				return;
			}

			m_Surface = SDL_GetWindowSurface(m_Window.get());
			if (m_Surface == nullptr)
			{
				events_.trigger<Error>(Error("Acquiring window surface failed."));
				return;
			}
		}
			
		void Run(entt::dispatcher& events_) override
		{
			SDL_FillRect(m_Surface, NULL, SDL_MapRGB(m_Surface->format, 0x64, 0x95, 0xED));
			SDL_UpdateWindowSurface(m_Window.get());
		}

		void WindDown(entt::dispatcher& events_) override
		{
			spdlog::info("Winding down renderer");

			m_Window.reset();
			m_Surface = nullptr;
		}
	};
}