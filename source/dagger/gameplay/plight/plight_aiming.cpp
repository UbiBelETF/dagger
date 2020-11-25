#define _USE_MATH_DEFINES

#include "plight_aiming.h"

#include "core/graphics/sprite.h"
#include "core/input/inputs.h"

#include <cmath>
#include <math.h>

void plight::PlightAimingSystem::OnInitialize(Registry& registry_, Entity entity_)
{
	InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
	for (auto command : { "rotate","moveY", "use" })
	{
		receiver.values[command] = 0;
	}
}

void plight::PlightAimingSystem::Run()
{
    Engine::Registry().view<InputReceiver, Sprite, PlightCrosshair>().each(
        [](const InputReceiver input_, Sprite& sprite_,PlightCrosshair& crosshair_)
        {
            Float32 rotate = input_.values.at("rotate");
           
                if (rotate) { //Change the angle of the crosshair
                    crosshair_.angle += rotate;
                    if (crosshair_.angle >= 2 * M_PI) {
                        crosshair_.angle -= 2 * M_PI;
                    }
                    else if (crosshair_.angle < 0) {
                        crosshair_.angle = 2 * M_PI - (-crosshair_.angle);
                    }

                    auto& sprite = Engine::Registry().get<Sprite>(crosshair_.crosshairSprite);

                    Float32 x = crosshair_.playerDistance * cos(crosshair_.angle);
                    Float32 y = crosshair_.playerDistance * sin(crosshair_.angle);


                    sprite.position.x = x + sprite_.position.x;
                    sprite.position.y = y + sprite_.position.y;

                    if (crosshair_.angle > 0.5 * M_PI && crosshair_.angle < 1.5 * M_PI) {
                        sprite_.scale = { -1,1 };
                    }
                    else {
                        sprite_.scale = { 1,1 };
                    }
                }
               
            
        });
}

void plight::PlightAimingSystem::SpinUp()
{
	Engine::Registry().on_construct<InputReceiver>().connect<&PlightAimingSystem::OnInitialize>(this);
}

void plight::PlightAimingSystem::WindDown()
{
	Engine::Registry().on_construct<InputReceiver>().disconnect<&PlightAimingSystem::OnInitialize>(this);
}
