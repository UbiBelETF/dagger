#include "player_controller.h"
#include "shoot.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

using namespace lab;


void PlayerControllerSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "rightleft", "updown", "shoot", "heavy", "light", "use","block"})
    {
        receiver.values[command] = 0;
    }
    
}

void PlayerControllerSystem::SpinUp()
{
    Engine::Registry().on_construct<InputReceiver>().connect<&PlayerControllerSystem::OnInitialize>(this);
}

void PlayerControllerSystem::Run()
{
    Engine::Registry().view<InputReceiver, Sprite, Animator, PlayerCharacter>().each(
        [](const InputReceiver input_, Sprite& sprite_, Animator& animator_, /*const*/ PlayerCharacter& char_)
        {
        
            Float32 rl = input_.values.at("rightleft");
            Float32 ud = input_.values.at("updown");
            Float32 shoot = input_.values.at("shoot");
        
            if(rl || ud)
            {
                AnimatorPlay(animator_, "main_character:run");
            if (rl != 0)
            { 
                sprite_.scale.x = rl;
                sprite_.position.x += char_.speed * sprite_.scale.x * Engine::DeltaTime();
            } 
            if (ud != 0)
            { 
                sprite_.scale.y = 1;
                sprite_.position.y += char_.speed * ud * Engine::DeltaTime();
            } 
            }
                
           else
            {
                AnimatorPlay(animator_, "main_character:idle");
            }
            
            if (shoot)
            {
                if (char_.cooldown <= 0)
                {
                    auto cursorInWindow = dagger::Input::CursorPositionInWorld();
                    Vector2 directions = { 1, 1 };
                    if (cursorInWindow.x < sprite_.position.x)
                        directions.x = -1;
                    else if (cursorInWindow.x == sprite_.position.x)
                        directions.x = 0;

                    if (cursorInWindow.y < sprite_.position.y)
                        directions.y = -1;
                    else if (cursorInWindow.y == sprite_.position.y)
                        directions.y = 0;

                    Float32 distanceX = cursorInWindow.x - sprite_.position.x;
                    Float32 distanceY = cursorInWindow.y - sprite_.position.y;
                    Float32 ratio = distanceY / distanceX;
                    ratio *= (ratio > 0) ? 1 : -1;

                    Vector2 position = { sprite_.position.x, sprite_.position.y };
                    CreateBullet(position, ratio, directions);
                    char_.cooldown = 100;
                }
            }
            char_.cooldown--;
        });
}

void PlayerControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&PlayerControllerSystem::OnInitialize>(this);
}
