#define _USE_MATH_DEFINES

#include "plight_controller.h"
#include "plight_game_logic.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"


#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include <cmath>
#include <math.h>
#include "gameplay/plight/plight_combat.h"
#include "gameplay/plight/plight_aiming.h"


using namespace plight;


void PlightControllerSystem::Run()
{
	auto viewLS1 = Engine::Registry().view<PlightIntro>();
	auto it = viewLS1.begin();
	auto& pin = viewLS1.get<PlightIntro>(*it);

	if (pin.IsFinished()) 
	{
		Engine::Registry().view<PlightCharacterControllerFSM::StateComponent>()
			.each([&](PlightCharacterControllerFSM::StateComponent& state_)
		{
			characterFSM.Run(state_);
		});
	}
   
}

