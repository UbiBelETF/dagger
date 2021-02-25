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
	
	void Set(String font_, String message_, Vector3 pos_,Vector2 size_);
	void Set(String font_, String message_);
	void Set(String font_, String message_,Vector2 size_);

	void Set(String font_, String message_, Vector3 pos_ = { 0, 0, 0 }, Bool ui_ = true);

};