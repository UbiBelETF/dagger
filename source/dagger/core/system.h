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
	};

	template<typename... Ts>
	struct Publisher
	{};

	template<typename... Ts>
	struct Subscriber
	{};
}