#include "idle_state.h"

#include "core/engine.h"
#include "core/input/inputs.h"

#include "gameplay/team_game/character_controller.h"

void IdleStateSystem::OnInitialize(Registry& registry_, Entity entity_)
{
	InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
	for (auto command : { "horizontalRun", "verticalRun" })
	{
		receiver.values[command] = 0;
	}
}

void IdleStateSystem::SpinUp()
{
	Engine::Registry().on_construct<InputReceiver>().connect<&IdleStateSystem::OnInitialize>(this);
}

void IdleStateSystem::Run()
{
	Engine::Registry().view<CharacterController, InputReceiver>().each(
		[](CharacterController controller_, const InputReceiver input_)
		{
			if (controller_.state == ECharacterState::Idle)
			{
				Float32 horizontal = input_.values.at("horizontalRun");
				Float32 vertical = input_.values.at("verticalRun");

				if (horizontal != 0 || vertical != 0)
				{
					controller_.state = ECharacterState::Running;
				}
			}
		});
}

void IdleStateSystem::WindDown()
{
	Engine::Registry().on_construct<InputReceiver>().disconnect<&IdleStateSystem::OnInitialize>(this);
}
