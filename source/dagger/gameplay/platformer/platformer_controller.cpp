#include "platformer_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

using namespace platformer;

void PlatformerControllerSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "run", "jump", "down", "roll", "heavy", "light", "use" })
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
    Engine::Registry().view<InputReceiver, Sprite, Animator, PlatformerCharacter>().each(
        [](const InputReceiver input_, Sprite& sprite_, Animator& animator_, PlatformerCharacter& char_)
        {
            Float32 roll = input_.values.at("roll");
            Float32 run = input_.values.at("run");

            if (roll == 1)
            {
                char_.isRolling = true;

                // Ex: If the player is pressing the left movement key and roll Key
                // allow him to do that. Alligning the sprite_.scale.x with run variable enables
                // the character to start rolling in the direction it is facing
                if (run != 0)
                {
                    sprite_.scale.x = run;
                }
            }
            else
            {
                if (char_.timeRolling >= char_.rollingTime)
                {
                    char_.isRolling = false;
                }
            }

            if (char_.isRolling)
            {
                if (char_.timeRolling < char_.rollingTime)
                {
                    AnimatorPlay(animator_, "souls_like_knight_character:ROLL");
                    sprite_.position.x += char_.rollingSpeed * sprite_.scale.x * Engine::DeltaTime();
                    char_.timeRolling += Engine::DeltaTime();
                }
                else
                {
                    char_.isRolling = false;
                    char_.timeRolling = 0;
                }
            }
            else
            {
                if (run == 0 && roll == 0)
                {
                    AnimatorPlay(animator_, "souls_like_knight_character:IDLE");
                }
                else
                {
                    AnimatorPlay(animator_, "souls_like_knight_character:RUN");
                    sprite_.scale.x = run;
                    sprite_.position.x += char_.speed * sprite_.scale.x * Engine::DeltaTime();
                }
            }
        });
}

void PlatformerControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&PlatformerControllerSystem::OnInitialize>(this);
}
