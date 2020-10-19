#pragma once

#include "structures.h"

class Texture;

struct Sprite
{
	Position3D m_Position{ 0, 0, 0 };		 // 3
	Position2D m_Pivot{ 0, 0 };				 // 2
	Color m_Color{ 1.0f, 1.0f, 1.0f, 1.0f }; // 4
	Float32 m_Image{ 0 };					 // 1
	Float32 m_Ratio{ 1.0f };				 // 1
	Float32 m_Scale{ 1.0f };				 // 1
											 // ==== sum ==
											 // 12

	void Use(String textureName_);
};
