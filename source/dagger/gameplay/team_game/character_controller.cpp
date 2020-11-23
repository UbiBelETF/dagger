#include "character_controller.h"

#include "gameplay/team_game/character_controller_states/idle_state.h"
#include "gameplay/team_game/character_controller_states/running_state.h"

void CharacterControllerSystem::SpinUp()
{
	this->AddSystem<IdleStateSystem>();
	this->AddSystem<RunningStateSystem>();

	for (auto& system : this->m_Systems)
	{
		system->SpinUp();
	}
}

void CharacterControllerSystem::Run()
{
	for (auto& system : this->m_Systems)
	{
		system->Run();
	}
}

void CharacterControllerSystem::WindDown()
{
	for (auto& system : this->m_Systems)
	{
		system->WindDown();
	}

	this->m_Systems.clear();
}
