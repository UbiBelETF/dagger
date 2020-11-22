#include "topdown_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

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
    Engine::Registry().view<InputReceiver, Sprite, Animator, TopdownCharacter>().each(
        [](const InputReceiver input_, Sprite& sprite_, Animator& animator_, const TopdownCharacter& char_)
        {
            Float32 moveX = input_.values.at("moveX");
            Float32 moveY = input_.values.at("moveY");
            if ((moveX || moveY) == 0)
            {
                //Idle
                AnimatorPlay(animator_, "Plight:big_deamon:IDLE");
            }
            else
            {
                //Walking
                AnimatorPlay(animator_, "Plight:big_deamon:RUN");
                if (moveX) sprite_.scale.x = moveX;
                sprite_.position.x += char_.speed * moveX * Engine::DeltaTime();
                sprite_.position.y += char_.speed * moveY * Engine::DeltaTime();
            }
        });
}

void TopdownControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&TopdownControllerSystem::OnInitialize>(this);
}
