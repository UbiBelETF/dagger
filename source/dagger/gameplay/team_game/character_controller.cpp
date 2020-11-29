#include "character_controller.h"
#include "gameplay/team_game/team_game_main.h"

#include "core/game/transforms.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

using namespace team_game;

void TeamGameControllerSystem::OnInitialize(Registry& registry_, Entity entity_)
{
    InputReceiver& receiver = registry_.get<InputReceiver>(entity_);
    for (auto command : { "run", "jump", "down", "heavy", "light", "use" })
    {
        receiver.values[command] = 0;
    }
}

void TeamGameControllerSystem::SpinUp()
{
    Engine::Registry().on_construct<InputReceiver>().connect<&TeamGameControllerSystem::OnInitialize>(this);
        Engine::Dispatcher().sink<KeyboardEvent>().connect<&TeamGameControllerSystem::OnKeyboardEvent>(this);
}

void TeamGameControllerSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        if (kEvent_.key == ctrl_.leftKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.x = -1;
        }
        else if (kEvent_.key == ctrl_.leftKey && kEvent_.action == EDaggerInputState::Released)
        {
            ctrl_.input.x = 0;
        }
        else if (kEvent_.key == ctrl_.rightKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.x = 1;
        }
        else if (kEvent_.key == ctrl_.rightKey && kEvent_.action == EDaggerInputState::Released)
        {
            ctrl_.input.x = 0;
        }
        else if (kEvent_.key == ctrl_.downKey && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
        {
            ctrl_.input.y = -1;
        }
        else if (kEvent_.key == ctrl_.downKey && kEvent_.action == EDaggerInputState::Released)
        {
            ctrl_.input.y = 0;
        }
        else if (kEvent_.key == ctrl_.upKey && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
        {
            ctrl_.input.y = 1;
        }
        else if (kEvent_.key == ctrl_.upKey && kEvent_.action == EDaggerInputState::Released)
        {
            ctrl_.input.y = 0;
        }
    });
}

void TeamGameControllerSystem::Run()
{

    Engine::Registry().view<Transform, ControllerMapping, Sprite, Animator, TeamGameCharacter>().each(
        [](Transform& transform_,ControllerMapping& ctrl_, Sprite& sprite_, Animator& animator_, const TeamGameCharacter& char_)
    {
        if (ctrl_.input.x == 0 && ctrl_.input.y == 0)
        {
            //AnimatorPlay(animator_, "souls_like_knight_character:IDLE");
        }
        else
        {
            //AnimatorPlay(animator_, "souls_like_knight_character:RUN");

            if(ctrl_.input.x != 0)
                sprite_.scale.x = ctrl_.input.x;
            transform_.position.x += ctrl_.input.x * char_.speed * Engine::DeltaTime();
            transform_.position.y += ctrl_.input.y * char_.speed * Engine::DeltaTime();
        }
    });
}

void TeamGameControllerSystem::WindDown()
{
    Engine::Registry().on_construct<InputReceiver>().disconnect<&TeamGameControllerSystem::OnInitialize>(this);
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&TeamGameControllerSystem::OnKeyboardEvent>(this);
}