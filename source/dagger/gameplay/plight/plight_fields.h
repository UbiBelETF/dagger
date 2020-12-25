#pragma once

#include "core/system.h"
#include "core/core.h"
using namespace dagger;

namespace plight {
	struct DefenseField {
		UINT32 fieldsLeft = 2;

		Float32 fieldSize = 50.f;

		Float32 initialStaminaCost = 20.f;
		Float32 initialHealthReturn = 20.f;

		bool initialDone = false;
		bool destroy = false;

		bool fieldActive = false;
		Float32 fieldDurationTime{ 10.f };
		Float32 currendDurationTime{ 0.f };

		Entity defenseFieldE;
		Entity holder;
	};



	class FieldsSystem
		: public System
	{
		inline String SystemName() { return "Fields System"; }

		void SpinUp() override;
		void Run() override;
		void WindDown() override;



	public:


	private:
		void OnEndOfFrame();

	};

}

