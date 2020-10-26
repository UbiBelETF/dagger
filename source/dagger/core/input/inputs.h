#pragma once

#include "core/core.h"
#include "core/system.h"

namespace dagger
{
	struct InputMapping
	{
		Bool Keys[256];
		Bool Mouse[10];
	};

	struct InputContext
	{
		virtual void ConsumeInput(InputMapping& mapping_) = 0;
	};

	class InputSystem
		: public System
	{
		void OnKeyboardEvent(KeyboardEvent input_);
		void OnMouseEvent(MouseEvent input_);

	public:
		InputMapping m_Mapping;
		Sequence<InputContext> m_InputContexts;

		inline String SystemName() { return "Input System"; }

		void SpinUp() override;
		void Run() override;
		void WindDown() override;
	};
}