#pragma once

#include "core/core.h"

enum struct TextAlignment
{
	LEFT,
	CENTER,
	RIGHT
};

struct Text
{
	String font;
	TextAlignment alignment{ TextAlignment::CENTER };
	Float32 spacing{ 1.0f };
	Sequence<Entity> entities;

	Vector3 position;

	inline void Set(String font_, String message_)
	{
		Set(font_, message_, { 0, 0, 0 });
	}

	void Set(String font_, String message_, Vector3 pos_);
};