#pragma once

#include "core/core.h"
#include "core/system.h"

#include <bitset>

namespace dagger
{
	struct InputPattern
	{
		Bool IsInputDown(DaggerKey key) const;
		Bool IsInputDown(DaggerMouse button) const;

		UInt32 GetInputLength(DaggerKey key) const;
		UInt32 GetInputLength(DaggerMouse mouse) const;

	private:
		Bool keys[256]{ false, };
		Bool mouse[10]{ false, };
		Map<UInt32, TimePoint> moment{};

		friend class InputSystem;
	};

	class InputSystem
		: public System
	{
		void Test(InputPattern pattern_);
		void OnKeyboardEvent(KeyboardEvent input_);
		void OnMouseEvent(MouseEvent input_);

	public:
		InputPattern m_CurrentInputPattern;
		std::bitset<267> m_Bitmap;

		inline String SystemName() { return "Input System"; }

		void SpinUp() override;
		void Run() override;
		void WindDown() override;
	};
}