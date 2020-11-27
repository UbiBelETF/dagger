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
            
            if(!char_.jump || char_.attack){
                if (char_.fall)
                {
                    sprite_.position.y -= char_.speed *2* Engine::DeltaTime();
                    char_.fall_time -= Engine::DeltaTime();
                    if (char_.fall_time <= 0) {
                        char_.fall = false;
                        char_.fall_time = 0;
                    }
                }
                if(attack|| char_.attack){
                    if(!char_.attack)
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
                else {
                    if (jump == 1 && !char_.fall)
                    {
                        char_.jump = true;
                        char_.jump_time = 0.5;
                        sprite_.position.y += char_.speed * Engine::DeltaTime();
                    }
                    if (run != 0)
                    {
                        char_.run = true;
                        sprite_.scale.x = run;
                        sprite_.position.x += char_.speed * sprite_.scale.x * Engine::DeltaTime();
                    }
                }
                
            } 
            else
            {
                if(char_.jump) 
                {
                    char_.jump_time -= Engine::DeltaTime();
                    sprite_.position.y += char_.speed * Engine::DeltaTime();
                    if (char_.jump_time <= 0) {
                        char_.jump = false;
                        char_.jump_time = 0;
                        char_.fall = true;
                        char_.fall_time = 0.25;
                      
                    }
                }
                if (char_.fall)
                {
                    sprite_.position.y -= char_.speed * 2*Engine::DeltaTime();
                    char_.fall_time -= Engine::DeltaTime();
                    if (char_.fall_time <= 0) {
                        char_.fall = false;
                        char_.fall_time = 0;
                    }
                }
                if(run!=0)
                {
                    char_.run = true;
                    sprite_.scale.x = run;
                    sprite_.position.x += char_.speed * sprite_.scale.x * Engine::DeltaTime();
                }
            }
            if (char_.attack) AnimatorPlay(animator_, "character:ATTACK");
            else if (char_.fall || char_.jump) AnimatorPlay(animator_, "character:JUMP");
            else if (char_.run) {
                char_.run = false; AnimatorPlay(animator_, "character:RUN");
            }
            else AnimatorPlay(animator_, "character:IDLE");
                


        });
}

void BrawlerControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&BrawlerControllerSystem::OnInitialize>(this);
}
