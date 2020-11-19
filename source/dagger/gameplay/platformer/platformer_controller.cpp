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
    for (auto command : { "run", "jump", "down", "heavy", "light", "use","block"})
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
        [](const InputReceiver input_, Sprite& sprite_, Animator& animator_, /*const*/ PlatformerCharacter& char_)
        {
            Float32 down=input_.values.at("down");
            Float32 run = input_.values.at("run");
            Float32 useP = input_.values.at("use");
            Float32 block = input_.values.at("block");
            Float32 sAttack = input_.values.at("heavy");

            if(down && run)        
            {          
                sprite_.position.x += char_.speed * sprite_.scale.x * Engine::DeltaTime();
                AnimatorPlay(animator_, "souls_like_knight_character:ROLL");
            } 
            else if (sAttack != 0 && block==0)
            {
                AnimatorPlay(animator_, "souls_like_knight_character:ATTACK");
                
            }
            else if (block != 0)
            {
                AnimatorPlay(animator_, "souls_like_knight_character:BLOCK");
                
            }
            else if (useP != 0 && run==0)
            {
                AnimatorPlay(animator_, "souls_like_knight_character:DRINK_POTION");
            } 
            else if (run != 0)
            { 
                AnimatorPlay(animator_, "souls_like_knight_character:RUN");
                sprite_.scale.x = run;
                sprite_.position.x += char_.speed * sprite_.scale.x * Engine::DeltaTime();
            }     
           
            
            Float32 jump = 0;
            if (!char_.jumping)
                jump = input_.values.at("jump");
            if (jump)
            {
                char_.jumping = true;
            }

            if (char_.jumping)
            {
                if (char_.jumpDuration <= char_.maxHeight)
                {
                    sprite_.position.y += char_.speed * sprite_.scale.y * Engine::DeltaTime();
                    AnimatorPlay(animator_, "souls_like_knight_character:JUMP");
                    char_.jumpDuration++;
                }
                else if (char_.jumpDuration <= 2 * char_.maxHeight)
                {
                    sprite_.position.y -= char_.speed * sprite_.scale.y * Engine::DeltaTime();
                    AnimatorPlay(animator_, "souls_like_knight_character:IDLE");
                    char_.jumpDuration++;
                }
                else
                {
                    char_.jumpDuration = 0;
                    char_.jumping = false;
                    sprite_.position.y = 0;
                }
            }
           if(run==0 && useP==0 && down==0 && block==0 && sAttack==0 && jump==0) 
            {
                AnimatorPlay(animator_, "souls_like_knight_character:IDLE");
            }
            

        });
}

void PlatformerControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&PlatformerControllerSystem::OnInitialize>(this);
}
