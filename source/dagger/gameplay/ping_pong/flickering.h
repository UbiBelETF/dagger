#pragma once
#include "core/system.h"
#include "core/core.h"

#include "core/graphics/sprite.h"

using namespace dagger;

struct Flickering
{
	Bool is_on{ false };

	Float32 transition_period{ 0.1 };
	Float32 time_left{ 0.0 };

	ColorRGBA on_color{ 1.0f, 0.0f, 0.0f, 1.0f };
	ColorRGBA off_color{ 1.0f, 1.0f, 1.0f, 1.0f };
};

class FlickeringSystem: public System
{
public:

	inline String SystemName() { return "Flickering System "; }

	void Run() override;

	void ChangeColor(Sprite& sprite, Flickering& flicker, Bool is_on);
};
