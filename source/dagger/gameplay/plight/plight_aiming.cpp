#define _USE_MATH_DEFINES

#include "plight_aiming.h"
#include "plight_game_logic.h"

#include "core/graphics/sprite.h"
#include "core/input/inputs.h"

#include <cmath>
#include <math.h>


void plight::PlightAimingSystem::Run()
{
	auto viewLS1 = Engine::Registry().view<PlightIntro>();
	auto it = viewLS1.begin();
	auto& pin = viewLS1.get<PlightIntro>(*it);

	if (pin.IsFinished()) {

		Engine::Registry().view<InputReceiver, Sprite, PlightCrosshair>().each(
			[](InputReceiver input_, Sprite& sprite_, PlightCrosshair& crosshair_)
		{

			Float32 rotate = input_.Get("rotate");

			if (rotate) { //Change the angle of the crosshair
				crosshair_.angle += rotate * crosshair_.rotationSpeed * Engine::DeltaTime();
				if (crosshair_.angle >= 2 * M_PI) {
					crosshair_.angle -= 2 * M_PI;
				}
				else if (crosshair_.angle < 0) {
					crosshair_.angle = 2 * M_PI - (-crosshair_.angle);
				}

				if (crosshair_.angle > 0.5 * M_PI && crosshair_.angle < 1.5 * M_PI) {
					sprite_.scale = { -1,1 };
				}
				else {
					sprite_.scale = { 1,1 };
				}

			}
            auto& sprite = Engine::Registry().get<Sprite>(crosshair_.crosshairSprite);

            Float32 x = crosshair_.playerDistance * cos(crosshair_.angle);
            Float32 y = crosshair_.playerDistance * sin(crosshair_.angle);


            sprite.position.x = x + sprite_.position.x;
            sprite.position.y = y + sprite_.position.y;

		});
	}

}
