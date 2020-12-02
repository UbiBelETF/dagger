#include "team_game_player_input.h"

#include "core/engine.h"
#include "gameplay/team_game/game_manager.h"

using namespace dagger;
using namespace team_game;

void TeamGamePlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&TeamGamePlayerInputSystem::OnKeyboardEvent>(this);
}

void TeamGamePlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&TeamGamePlayerInputSystem::OnKeyboardEvent>(this);
}

void TeamGamePlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    if (kEvent_.key == nextLevelKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
    {
        triggeredTransition = true;
    }
}

void TeamGamePlayerInputSystem::Run()
{
    if (triggeredTransition)
    {
        GameManagerSystem::completedObjective = true;
        triggeredTransition = false;
    }
}
