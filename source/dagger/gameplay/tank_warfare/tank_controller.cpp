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
	static std::string s_lastState = "jovanovici:tank:tank3_side";
	Engine::Registry().view<InputReceiver, Sprite, Animator, Transform, TankCharacter>().each(
		[](const InputReceiver input_, Sprite& sprite_, Animator& animator_, Transform& transform_, const TankCharacter& tank_)
		{
			Float32 driveLR = input_.values.at("driveLeftRight");
			Float32 driveUD = input_.values.at("driveUpDown");

			if (driveLR == 0 && driveUD == 0)
			{
			//	AnimatorPlay(animator_, "tank:idle");
				AssignSpriteTexture(sprite_, s_lastState);
			}
			if (driveLR != 0)
			{
				AnimatorPlay(animator_, "tank:driveLeftRight");
				sprite_.scale.x = driveLR * (-1);
				sprite_.position.x += tank_.speed * driveLR * Engine::DeltaTime();
				transform_.position.x += tank_.speed * driveLR * Engine::DeltaTime();
				s_lastState = "jovanovici:tank:tank3_side";
			}
			else if (driveUD != 0)
			{
				//need to add animations
				if (driveUD == 1)
				{
					AssignSpriteTexture(sprite_, "jovanovici:tank:tank3_back");
					s_lastState = "jovanovici:tank:tank3_back";
				}
				else
				{
					AssignSpriteTexture(sprite_, "jovanovici:tank:tank3_front");
					s_lastState = "jovanovici:tank:tank3_front";
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
