#pragma once

#include "core/core.h"
#include "core/graphics/shader.h"
#include "core/graphics/texture.h"

namespace dagger
{
	struct SpriteFrame;

	struct SpriteCutoutData
	{
		Vector2 subSize{ 1.0f, 1.0f };					// 2
		Vector2 subOrigin{ 0.0f, 0.0f };				// 2
		Vector2 size{ 1.0f, 1.0f };						// 2

		inline void UseFullImage()
		{
			subSize = { 1.0, 1.0f };
			subOrigin = { 0.0f, 0.0f };
		}

		void Use(SpriteFrame* spritesheet_);
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

	struct SpriteFrame
	{
		ViewPtr<Texture> texture;
		SpriteCutoutData frame;
	};

	void AssignSprite(Sprite& spriteTarget_, String textureName_);
	void AssignSprite(Sprite& spriteTarget_, ViewPtr<Texture> texture_);
	void AssignSprite(Sprite& spriteTarget_, ViewPtr<SpriteFrame> texture_);

	void AssignSpriteShader(Sprite& spriteTarget_, String shaderName_);

}