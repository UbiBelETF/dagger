#pragma once

#include "core/core.h"

struct Text
{
	String font;
	Float32 spacing{ 1.0f };
	Sequence<Entity> entities;

	Vector3 position;

	void Set(String font, String message_, Vector3 pos_ = { 0, 0, 0 });
};