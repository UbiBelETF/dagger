#pragma once

#include "core/core.h"
#include "core/graphics/shader.h"
#include "core/graphics/texture.h"

namespace dagger
{
	struct SpriteCutoutData
	{
		Vector2 subSize{ 1.0f, 1.0f };					// 2
		Vector2 subOrigin{ 0.0f, 0.0f };				// 2
		Vector2 size{ 1.0f, 1.0f };						// 2
	};

	struct SpriteData : public SpriteCutoutData
	{
		Vector3 position{ 0, 0, 0 };					// 3
		Vector2 pivot{ 0, 0 };							// 2
		ColorRGBA color{ 1.0f, 1.0f, 1.0f, 1.0f };		// 4
		Vector2 scale{ 1.0f, 1.0f };					// 2
	};

	struct Sprite : public SpriteData
	{
		Texture* image{ nullptr };
		ViewPtr<Shader> shader{ Shader::s_FirstLoadedShader };
	};

	void AssignSpriteTexture(Sprite& spriteTarget_, String textureName_);
	void AssignSpriteTexture(Sprite& spriteTarget_, ViewPtr<Texture> texture_);

	void AssignSpriteShader(Sprite& spriteTarget_, String shaderName_);
}