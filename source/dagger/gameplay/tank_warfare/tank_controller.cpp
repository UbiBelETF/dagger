#include "tank_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

using namespace tank_warfare;

void TankControllerSystem::OnInitialize(Registry& registry_, Entity entity_)
{
	InputReceiver& reciever = registry_.get<InputReceiver>(entity_);
	for (auto command : { "driveLeftRight", "driveUpDown", "fire" })
	{
		reciever.values[command] = 0;
	}
}

void TankControllerSystem::SpinUp()
{
	Engine::Registry().on_construct<InputReceiver>().connect<&TankControllerSystem::OnInitialize>(this);
}

void TankControllerSystem::Run()
{
	Engine::Registry().view<InputReceiver, Sprite, Animator, Transform, TankCharacter>().each(
		[](const InputReceiver input_, Sprite& sprite_, Animator& animator_, Transform& transform_, TankCharacter& tank_)
		{
			Float32 driveLR = input_.values.at("driveLeftRight");
			Float32 driveUD = input_.values.at("driveUpDown");

			if (driveLR == 0 && driveUD == 0)
			{
				switch (tank_.lastOrientation)
				{
				case 0: AnimatorPlay(animator_, "tank:idleLeftRight"); break;
				case 1: AssignSpriteTexture(sprite_, "jovanovici:tank:tank3_back"); break;
				case 2: AssignSpriteTexture(sprite_, "jovanovici:tank:tank3_front"); break;
				}
			}
			if (driveLR != 0)
			{
				AnimatorPlay(animator_, "tank:driveLeftRight");
				tank_.lastOrientation = 0;
				sprite_.scale.x = driveLR * (-1);
				sprite_.position.x += tank_.speed * driveLR * Engine::DeltaTime();
				transform_.position.x += tank_.speed * driveLR * Engine::DeltaTime();
			}
			else if (driveUD != 0)
			{
				if (driveUD == 1)
				{
					AnimatorPlay(animator_, "tank:driveUp");
					tank_.lastOrientation = 1;
				}
				else
				{
					AnimatorPlay(animator_, "tank:driveDown");
					tank_.lastOrientation = 2;
				}
				sprite_.position.y += tank_.speed * driveUD * Engine::DeltaTime();
				transform_.position.y += tank_.speed * driveUD * Engine::DeltaTime();
			}
		});
}

void TankControllerSystem::WindDown()
{
	Engine::Registry().on_construct<InputReceiver>().disconnect<&TankControllerSystem::OnInitialize>(this);
}
