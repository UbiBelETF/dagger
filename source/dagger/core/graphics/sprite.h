#pragma once

#include "core/core.h"
#include "shader.h"

struct Sprite
{
	Vector3 m_Position{ 0, 0, 0 };			 // 3
	Vector2 m_Pivot{ 0, 0 };				 // 2
	Color m_Color{ 1.0f, 1.0f, 1.0f, 1.0f }; // 4
	Float32 m_Image{ 0 };					 // 1
	Float32 m_Ratio{ 1.0f };				 // 1
	Float32 m_Scale{ 1.0f };				 // 1
											 // ==== sum ==
											 // 12
	void UseTexture(String textureName_);
};
