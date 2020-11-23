#include "topdown_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

#include "gameplay/plight/plight_combat.h"

using namespace plight;

void TopdownControllerSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "moveX", "moveY", "use" })
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
    Engine::Registry().view<InputReceiver, Sprite,Transform, Animator, TopdownCharacter, CombatStats>().each(
        [](const InputReceiver input_, Sprite& sprite_,Transform& transform_, Animator& animator_,TopdownCharacter& char_, CombatStats& cstats_)
        {
            Float32 moveX = input_.values.at("moveX");
            Float32 moveY = input_.values.at("moveY");

            if (char_.resting) {
                AnimatorPlay(animator_, "Plight:big_deamon:IDLE");
                char_.currentRestingTime += Engine::DeltaTime();
                if (char_.currentRestingTime >= char_.restingTime) {
                    char_.resting = false;
                    char_.currentRestingTime = 0.f;
                }
            }
            else if ((moveX == 0 && moveY == 0) || cstats_.currentStamina < STAMINA_FOR_RUNNING_FRAME)
            {
                //Idle
                AnimatorPlay(animator_, "Plight:big_deamon:IDLE");
                char_.running = false;
            }
            else if(cstats_.currentStamina >= STAMINA_FOR_RUNNING_FRAME && !char_.resting){
                //Walking
                AnimatorPlay(animator_, "Plight:big_deamon:RUN");
                char_.running = true;
                if (moveX) sprite_.scale.x = moveX;
                sprite_.position.x += char_.speed * moveX * Engine::DeltaTime();
                transform_.position.x += char_.speed * moveX * Engine::DeltaTime();

                sprite_.position.y += char_.speed * moveY * Engine::DeltaTime();
                transform_.position.y += char_.speed * moveY * Engine::DeltaTime();
            }
        });
}

void TopdownControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&TopdownControllerSystem::OnInitialize>(this);
}
