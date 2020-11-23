#include "character_controller.h"

#include "core/engine.h"

#include "gameplay/team_game/character_controller_states/idle_state.h"
#include "gameplay/team_game/character_controller_states/running_state.h"

int CharacterController::activeStateCount[2] = { 0, 0 };

void CharacterControllerSystem::SpinUp()
{
	this->AddSystem<IdleStateSystem>(ECharacterState::Idle);
	this->AddSystem<RunningStateSystem>(ECharacterState::Running);

	for (auto& system : this->m_Systems)
	{
		system.second->SpinUp();
	}
}

void CharacterControllerSystem::Run()
{
	for (auto& [state, system] : this->m_Systems)
	{
		if (CharacterController::activeStateCount[(int)state] > 0)
		{
			system->Run();
		}
	}
}

void CharacterControllerSystem::WindDown()
{
	for (auto& system : this->m_Systems)
	{
		system.second->WindDown();
	}

	this->m_Systems.clear();
}
