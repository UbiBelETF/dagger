#pragma once

#include "core/core.h"
#include "core/graphics/shader.h"
#include "core/graphics/texture.h"

namespace dagger
{
	struct Sprite
	{
		Vector3 position{ 0, 0, 0 };
		Vector2 pivot{ 0, 0 };
		Color   color{ 1.0f, 1.0f, 1.0f, 1.0f };
		Float32 image{ 0 };
		Float32 ratio{ 1.0f };
		Float32 scale{ 1.0f };
	};

	void AssignSpriteTexture(Sprite& spriteTarget_, String textureName_);
	void AssignSpriteTexture(Sprite& spriteTarget_, ViewPtr<Texture> texture_);
}