#pragma once

#include <entt/entt.hpp>
#include <string>

namespace dagger
{
	struct Frame
	{};

	struct Exit
	{};

	struct Error
	{
		std::string m_Message;

		Error(std::string message_)
			: m_Message{ message_ }
		{}

		Error() = delete;

		Error(const Error&) = default;
		Error& operator=(const Error&) = default;
		Error(Error&&) = default;
		Error& operator=(Error&&) = default;

		~Error() = default;
	};

	struct System
	{
		virtual void SpinUp(entt::dispatcher&) = 0;
		virtual void Run(entt::dispatcher&) = 0;
		virtual void WindDown(entt::dispatcher&) = 0;
	};
}