#include "enemy_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

void team_game::EnemyControllerSystem::Run()
{
	Engine::Registry().view<EnemyControllerFSM::StateComponent>()
		.each([&](EnemyControllerFSM::StateComponent& state_)
			{
				enemyFSM.Run(state_);
			});
}
