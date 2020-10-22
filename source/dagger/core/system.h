#pragma once

#include <cassert>

namespace dagger
{
	class Engine;

	struct System
	{
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