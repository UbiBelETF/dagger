#pragma once
#include "core/system.h"
#include "core/core.h"

#include "core/graphics/sprite.h"

using namespace dagger;

namespace ping_pong
{
	struct Flickering
	{
		Bool isOn{ false };

		Float32 transitionPeriod{ 0.1f };
		Float32 timeLeft{ 0.0f };

		ColorRGBA onColor{ 1.0f, 0.0f, 0.0f, 1.0f };
		ColorRGBA offColor{ 1.0f, 1.0f, 1.0f, 1.0f };
	};

	class FlickeringSystem : public System
	{
	public:

		inline String SystemName() { return "Flickering System "; }

		void Run() override;

		void ChangeColor(Sprite& sprite_, Flickering& flickering_, Bool isOn_);
	};
};
