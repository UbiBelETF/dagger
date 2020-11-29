#include "physics.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "gameplay/team_game/brawler_controller.h"

Float32 team_game::PhysicsSystem::ground = -5;

void team_game::PhysicsSystem::SetGround(Float32 ground_)
{
    ground = ground_;
}

void team_game::PhysicsSystem::Run()
{
    Engine::Registry().view< Sprite, BrawlerCharacter>().each(
        []( Sprite& sprite_, BrawlerCharacter& char_)
        {
            if (char_.jump) 
            {
                sprite_.position.y += char_.speed*char_.jump_time*2 * Engine::DeltaTime();
                char_.jump_time -= Engine::DeltaTime();
                if(char_.jump_time<=0) 
                {
                    char_.jump = false;
                    char_.fall = true;
                }
            }
            else if (char_.fall) 
            {
                sprite_.position.y -= char_.speed * Engine::DeltaTime();
                if (sprite_.position.y + 10 <= ground)
                {
                    sprite_.position.y = -15;
                    char_.fall = false;
                    char_.double_jump = false;
                }
            }


            if (char_.run)
            {
                sprite_.position.x += char_.speed *sprite_.scale.x* Engine::DeltaTime();
                char_.run = false;
            }

        });
}
