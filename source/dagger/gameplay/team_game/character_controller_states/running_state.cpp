#include "running_state.h"

#include <math.h>

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"

#include "gameplay/team_game/character_controller.h"

using namespace std;

void RunningStateSystem::OnInitialize(Registry& registry_, Entity entity_)
{
	InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
	for (auto command : { "horizontalRun", "verticalRun" })
	{
		receiver.values[command] = 0;
	}
}

void RunningStateSystem::SpinUp()
{
	Engine::Registry().on_construct<InputReceiver>().connect<&RunningStateSystem::OnInitialize>(this);
}

void RunningStateSystem::Run()
{
	Engine::Registry().view<CharacterController, InputReceiver, Transform>().each(
		[](CharacterController& controller_, const InputReceiver input_, Transform& transform_)
		{
			if (controller_.state == ECharacterState::Running)
			{
				Float32 horizontal = input_.values.at("horizontalRun");
				Float32 vertical = input_.values.at("verticalRun");

				if (horizontal == 0 && vertical == 0)
				{
					controller_.state = ECharacterState::Idle;
					CharacterController::activeStateCount[(int)ECharacterState::Running]--;
					CharacterController::activeStateCount[(int)ECharacterState::Idle]++;
				}
				else
				{
					if (horizontal != 0 && vertical != 0)
					{
						horizontal /= sqrt(2);
						vertical /= sqrt(2);
					}
					transform_.position.x += horizontal * controller_.speed * Engine::DeltaTime();
					transform_.position.y += vertical * controller_.speed * Engine::DeltaTime();
				}
			}
		});
}

void RunningStateSystem::WindDown()
{
	Engine::Registry().on_construct<InputReceiver>().disconnect<&RunningStateSystem::OnInitialize>(this);
}
