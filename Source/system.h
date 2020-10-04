#pragma once

#include <cassert>

namespace dagger
{
	class Engine;

	struct System
	{
		virtual void SpinUp(Engine&) {};
		virtual void Run(Engine&) {};
		virtual void WindDown(Engine&) {};
	};

	template<typename... Ts>
	struct Publisher
	{};

	template<typename... Ts>
	struct Subscriber
	{};
}