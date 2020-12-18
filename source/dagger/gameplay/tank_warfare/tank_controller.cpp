#include "tank_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

#include "gameplay/tank_warfare/rocket.h"
#include "gameplay/common/simple_collisions.h"

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
	Engine::Registry().view<InputReceiver, Sprite, Animator, Transform, TankCharacter, SimpleCollision>().each(
		[](const InputReceiver input_, Sprite& sprite_, Animator& animator_, Transform& transform_, TankCharacter& tank_, SimpleCollision& col_)
	{
		Float32 driveLR = input_.values.at("driveLeftRight");
		Float32 driveUD = input_.values.at("driveUpDown");
		Float32 fire = input_.values.at("fire");
		Vector2 rocketPos = { transform_.position.x, transform_.position.y + 4 };
		Vector2 rocketSpeed = { 0, 0 };

		if (driveLR == 0 && driveUD == 0)
		{
			switch (tank_.lastOrientation)
			{
			case ETankOrientation::TankLeft: case ETankOrientation::TankRight:
			{
				AnimatorPlay(animator_, "tank:idleLeftRight");
				rocketSpeed = { 200 * -sprite_.scale.x, 0 };
				break;
			}
			case ETankOrientation::TankUp:
			{
				AssignSprite(sprite_, "jovanovici:tank:tank3_back");
				rocketSpeed = { 0, 200 };
				break;
			}
			case ETankOrientation::TankDown:
			{
				AssignSprite(sprite_, "jovanovici:tank:tank3_front");
				rocketSpeed = { 0, -200 };
				break;
			}
			}
			if (tank_.reloadTime >= 0)
			{
				tank_.reloadTime -= Engine::DeltaTime();
			}
			else if (fire != 0)
			{
				CreateRocket(rocketPos, rocketSpeed, tank_.id, tank_.lastOrientation);
				tank_.reloadTime = 1;
			}
		}
		if (driveLR != 0)
		{
			AnimatorPlay(animator_, "tank:driveLeftRight");
			if (driveLR == -1)
			{
				tank_.lastOrientation = ETankOrientation::TankLeft;
			}
			else
			{
				tank_.lastOrientation = ETankOrientation::TankRight;
			}
			col_.size = sprite_.size;
			sprite_.scale.x = driveLR * (-1);
			sprite_.position.x += tank_.speed * driveLR * Engine::DeltaTime();
			transform_.position.x += tank_.speed * driveLR * Engine::DeltaTime();
		}
		else if (driveUD != 0)
		{
			if (driveUD == 1)
			{
				AnimatorPlay(animator_, "tank:driveUp");
				tank_.lastOrientation = ETankOrientation::TankUp;
				col_.size = sprite_.size;
			}
			else
			{
				AnimatorPlay(animator_, "tank:driveDown");
				tank_.lastOrientation = ETankOrientation::TankDown;
				col_.size = sprite_.size;
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
