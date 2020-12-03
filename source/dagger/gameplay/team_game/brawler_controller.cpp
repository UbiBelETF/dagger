#include "brawler_controller.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"



#include "gameplay/team_game/physics.h"
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

    Engine::Registry().view<Physics,InputReceiver, Sprite, Animator, BrawlerCharacter>().each(
        [](Physics& physics_,const InputReceiver input_, Sprite& sprite_, Animator& animator_, BrawlerCharacter& char_)
        {

            Float32 run = input_.values.at("run");
            Float32 jump = input_.values.at("jump");
           // Float32 attack = input_.values.at("light");

            if (run != 0) {
                sprite_.scale.x = run;
                physics_.velocity.x += sprite_.scale.x* char_.speed;
                char_.run = true;
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
