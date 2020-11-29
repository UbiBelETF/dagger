#include "brawler_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

using namespace team_game;

void BrawlerControllerSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "run", "jump", "down", "heavy", "light", "use" })
    {
        receiver.values[command] = 0;
    }
}

void BrawlerControllerSystem::SpinUp()
{
    Engine::Registry().on_construct<InputReceiver>().connect<&BrawlerControllerSystem::OnInitialize>(this);
}

void BrawlerControllerSystem::Run()
{

    Engine::Registry().view<InputReceiver, Sprite, Animator, BrawlerCharacter>().each(
        [](const InputReceiver input_, Sprite& sprite_, Animator& animator_, BrawlerCharacter& char_)
        {

            Float32 run = input_.values.at("run");
            Float32 jump = input_.values.at("jump");
            Float32 attack = input_.values.at("light");

            if(!char_.jump )
            {
                if(jump==1 && char_.fall && !char_.double_jump)
                {
                    char_.double_jump = true;
                    char_.jump = true;
                    char_.jump_time = 1;
                }
                if(jump==1 && !char_.double_jump)
                {
                    if (!char_.double_jump)
                    {
                        char_.jump = true;
                        char_.jump_time = 1;
                    }
                }
                else if ((attack == 1 || char_.attack) && !char_.fall)
                {
                    if(char_.attack==false)
                    {
                        char_.attack = true;
                        char_.attack_time = 1;
                    }
                    else
                    {
                        char_.attack_time -= Engine::DeltaTime();
                        if (char_.attack_time <= 0) char_.attack = false;
                    }
                }
                else if (run != 0)
                {
                    sprite_.scale.x = run;
                    char_.run = true;
                }
            }
            else
            {
                if (run != 0)
                {
                    sprite_.scale.x = run;
                    char_.run = true;
                }
            }


            if (char_.attack) AnimatorPlay(animator_, "character:ATTACK");
            else if (char_.jump) AnimatorPlay(animator_, "character:JUMP");
            else if (char_.fall) AnimatorPlay(animator_, "character:FALL");
            else if (char_.run) AnimatorPlay(animator_, "character:RUN");
            else AnimatorPlay(animator_, "character:IDLE");






        });
}

void BrawlerControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&BrawlerControllerSystem::OnInitialize>(this);
}
