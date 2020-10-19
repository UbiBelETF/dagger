#pragma once

#include "core.h"
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
	UInt32 m_Shader;
	void UseTexture(String textureName_);
	void UseShader(String shaderName_);
};
