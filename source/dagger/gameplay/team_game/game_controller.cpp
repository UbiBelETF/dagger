#include "game_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"

#include "gameplay/team_game/team_game_main.h"

using namespace dagger;
using namespace team_game;

void GameControllerSystem::Run()
{
	Engine::Registry().view<GameController, InputReceiver>().each([](GameController& ctrl_, InputReceiver& input_)
		{
			if (EPSILON_NOT_ZERO(input_.Get("reload")))
			{
				Engine::Registry().clear();
				SetupWorld(Engine::Instance());
				Logger::info("RELOADING");
			}
		});
}
