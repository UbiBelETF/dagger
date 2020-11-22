#include "running_state.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"

#include "gameplay/team_game/character_controller.h"

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
	Engine::Registry().view<CharacterController, InputReceiver, Sprite>().each(
		[](CharacterController controller_, const InputReceiver input_, Sprite sprite_)
		{
			if (controller_.state == ECharacterState::Running)
			{
				Float32 horizontal = input_.values.at("horizontalRun");
				Float32 vertical = input_.values.at("verticalRun");

				if (horizontal == 0 && vertical == 0)
				{
					controller_.state = ECharacterState::Idle;
				}
				else
				{
					sprite_.position.x += horizontal * controller_.speed * Engine::DeltaTime();
					sprite_.position.y += vertical * controller_.speed * Engine::DeltaTime();
				}
			}
		});
}

void RunningStateSystem::WindDown()
{
	Engine::Registry().on_construct<InputReceiver>().disconnect<&RunningStateSystem::OnInitialize>(this);
}
