#define _USE_MATH_DEFINES

#include "topdown_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"


#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include <cmath>
#include <math.h>
#include "gameplay/plight/plight_combat.h"
#include "gameplay/plight/plight_aiming.h"


using namespace plight;

void TopdownControllerSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "rotate","moveY", "use" })
    {
        receiver.values[command] = 0;
    }
}

void TopdownControllerSystem::SpinUp()
{
    Engine::Registry().on_construct<InputReceiver>().connect<&TopdownControllerSystem::OnInitialize>(this);
}

void TopdownControllerSystem::Run()
{
    Engine::Registry().view<InputReceiver, Sprite,Transform, Animator, TopdownCharacter, CombatStats , PlightCrosshair>().each(
        [](const InputReceiver input_, Sprite& sprite_,Transform& transform_, Animator& animator_,TopdownCharacter& char_, CombatStats& cstats_, PlightCrosshair& crosshair_)
        {
            Float32 move = input_.values.at("moveY");

            if (char_.resting) {
                AnimatorPlay(animator_, "Plight:big_deamon:IDLE");
                char_.currentRestingTime += Engine::DeltaTime();
                if (char_.currentRestingTime >= char_.restingTime) {
                    char_.resting = false;
                    char_.currentRestingTime = 0.f;
                }
            }
            else if (move == 0 || cstats_.currentStamina < STAMINA_FOR_RUNNING_FRAME)
            {
                //Idle
                AnimatorPlay(animator_, "Plight:big_deamon:IDLE");
                char_.running = false;
            }
            else if(cstats_.currentStamina >= STAMINA_FOR_RUNNING_FRAME && !char_.resting){
                //Walking
                AnimatorPlay(animator_, "Plight:big_deamon:RUN");
                char_.running = true;

                if (move) {
                    Float32 dx = char_.speed * cos(crosshair_.angle) * Engine::DeltaTime() * move;
                    Float32 dy = char_.speed * sin(crosshair_.angle) * Engine::DeltaTime()* move;

                    sprite_.position.x += dx;
                    sprite_.position.y += dy;
                    transform_.position.x += dx;
                    transform_.position.y += dy;

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
            }
        });
}

void TopdownControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&TopdownControllerSystem::OnInitialize>(this);
}
