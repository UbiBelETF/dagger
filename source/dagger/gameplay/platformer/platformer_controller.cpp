#include "platformer_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

void PlatformerControllerSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "run", "jump", "down", "heavy", "light", "use" })
    {
        receiver.values[command] = 0;
    }
}

void PlatformerControllerSystem::SpinUp()
{
    Engine::Registry().on_construct<InputReceiver>().connect<&PlatformerControllerSystem::OnInitialize>(this);
}

void PlatformerControllerSystem::Run()
{
    Engine::Registry().view<InputReceiver, Sprite, Animator>().each(
        [](const InputReceiver input_, Sprite& sprite_, Animator& animator_)
        {
            Float32 run = input_.values.at("run");
            if (run == 0)
            {
                AnimatorPlay(animator_, "souls_like_knight_character:IDLE");
            }
            else
            {
                AnimatorPlay(animator_, "souls_like_knight_character:RUN");
                sprite_.scale.x = run;
                sprite_.position.x += 20 * sprite_.scale.x * Engine::DeltaTime();

                auto cam = Engine::GetDefaultResource<Camera>();
                cam->position = sprite_.position;
                cam->Update();
            }
        });
}

void PlatformerControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&PlatformerControllerSystem::OnInitialize>(this);
}
