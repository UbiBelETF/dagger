#pragma once

#include <cassert>

namespace dagger
{
	class Engine;

	struct System
	{
		virtual inline void SpinUp(Engine&) {};
		virtual inline void Run(Engine&) {};
		virtual inline void WindDown(Engine&) {};
	};

	template<typename... Ts>
	struct Publisher
	{};

	template<typename... Ts>
	struct Subscriber
	{};
}