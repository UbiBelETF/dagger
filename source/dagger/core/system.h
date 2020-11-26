#pragma once

#include "core/core.h"

#include <cassert>

namespace dagger
{
	class Engine;

	struct System
	{
		virtual String SystemName() = 0;
		virtual inline void SpinUp() {};
		virtual inline void Run() {};
		virtual inline void WindDown() {};

		Bool canBePaused{ false };
		Bool isPaused{ false };

		void Pause() { isPaused = true; }
		void Unpause() { isPaused = false; }
	};

	template<typename... Ts>
	struct Publisher
	{};

	template<typename... Ts>
	struct Subscriber
	{};
}