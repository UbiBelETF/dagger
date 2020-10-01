#pragma once

#include <SDL.h>

#include <atomic>
#include <utility>
#include <string>

struct TextureLoaded
{
	uint32_t m_Id;
	std::string m_Path;
	SDL_Texture* m_Texture;
};

struct LoadTextureRequest
{
	static std::atomic<uint32_t> ms_TextureId;

	std::string m_Path;

	TextureLoaded Load(SDL_Texture* texture_);
};

std::atomic<uint32_t> LoadTextureRequest::ms_TextureId{ 0 };

TextureLoaded LoadTextureRequest::Load(SDL_Texture* texture_)
{
	return TextureLoaded{ ++ms_TextureId, m_Path, texture_ };
}
