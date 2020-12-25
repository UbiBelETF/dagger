#include "controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/brawler_controller.h"
#include "gameplay/team_game/physics.h"
#include "gameplay/team_game/animations_fsm.h"
#include "gameplay/team_game/player_camera_focus.h"
#include "gameplay/team_game/team_game_main.h"
using namespace dagger;
using namespace team_game;

void team_game::ControllerFSM::Run(StateComponent& component_) 
{
    FSM<ECharacterStates>::Run(component_);
    auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(component_.entity);
    animationsFSM.Run(anim);
}
//idle
void ControllerFSM::Idle::Enter(ControllerFSM::StateComponent& state_) 
{
    auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
    ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Idle, anim);
}

void ControllerFSM::Idle::Run(ControllerFSM::StateComponent& state_) 
{

    auto&& [input,physics_,char_] = Engine::Registry().get<InputReceiver,Physics,BrawlerCharacter>(state_.entity);
    if (EPSILON_NOT_ZERO(char_.gotHit)) GoTo(ECharacterStates::Hitted, state_);

   
    else if (EPSILON_NOT_ZERO(input.Get("interact"))) GoTo(ECharacterStates::Interact, state_);

    else if (EPSILON_NOT_ZERO(input.Get("light"))) 
    {
        GoTo(ECharacterStates::Attacking, state_);
    }
    else if (EPSILON_NOT_ZERO(input.Get("run")) && EPSILON_ZERO(input.Get("jump")))
        GoTo(ECharacterStates::Running, state_);
    else if (EPSILON_NOT_ZERO(input.Get("jump"))|| physics_.velocity.y!=GetGravity()*Engine::DeltaTime() )
        GoTo(ECharacterStates::InAir, state_);
}

DEFAULT_EXIT(ControllerFSM, Idle);



//run
void ControllerFSM::Running::Enter(ControllerFSM::StateComponent& state_)
{
    auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
    ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Running, anim);
}
void ControllerFSM::Running::Run(ControllerFSM::StateComponent& state_) {
    auto& input = Engine::Registry().get<InputReceiver>(state_.entity);
    auto&& [sprite_, char_, physics_] = Engine::Registry().get<Sprite, BrawlerCharacter, Physics>(state_.entity);

    if (EPSILON_NOT_ZERO(char_.gotHit)) GoTo(ECharacterStates::Hitted, state_);

    else if (EPSILON_NOT_ZERO(input.Get("interact"))) GoTo(ECharacterStates::Interact, state_);

    else if (EPSILON_NOT_ZERO(input.Get("light")))
    {
        GoTo(ECharacterStates::Attacking, state_);
    }
    else if (EPSILON_ZERO(input.Get("run")) && EPSILON_ZERO(input.Get("jump")))
        GoTo(ECharacterStates::Idle, state_);
    else if (EPSILON_NOT_ZERO(input.Get("jump"))|| physics_.velocity.y != GetGravity() * Engine::DeltaTime()) {
        GoTo(ECharacterStates::InAir,state_);
    }
    else
    {
        sprite_.scale.x = input.values.at("run");
        physics_.velocity.x += char_.speed.x * sprite_.scale.x;
    }
}
DEFAULT_EXIT(ControllerFSM, Running);

//in_air
void ControllerFSM::InAir::Enter(ControllerFSM::StateComponent& state_)
{
    auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
    if(EPSILON_NOT_ZERO(input_.Get("jump")))
    {
        auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
            ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Jumping, anim);
    }
    else
    {
        auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
        ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Falling, anim);
    }
}
void ControllerFSM::InAir::Run(ControllerFSM::StateComponent& state_) 
{
    auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
    auto&& [sprite_, char_, physics_,transform_] = Engine::Registry().get<Sprite, BrawlerCharacter, Physics,Transform>(state_.entity);

    if (EPSILON_NOT_ZERO(char_.gotHit)) GoTo(ECharacterStates::Hitted, state_);
    else if (EPSILON_NOT_ZERO(input_.Get("jump")) && !char_.jump && physics_.velocity.y == GetGravity() * Engine::DeltaTime())

    {
        physics_.velocity.y += char_.speed.y;
        char_.jump = true;
        //char_.doubleJump = false;
        //JUMP ONLY WHEN RELEASED KEY(Not holding)
    }
    if (EPSILON_NOT_ZERO(input_.Get("jump")) && physics_.velocity.y <GetGravity()*Engine::DeltaTime()&& !char_.doubleJump)
    {
        char_.doubleJump = true;
        physics_.velocity.y = char_.speed.y;
    }
    if (EPSILON_NOT_ZERO(input_.Get("run"))) {
        sprite_.scale.x = input_.Get("run");
        physics_.velocity.x += char_.speed.x * sprite_.scale.x;
    }
    if (EPSILON_NOT_ZERO(input_.Get("run")) && physics_.velocity.y == GetGravity() * Engine::DeltaTime())
    {
        //char_.doubleJump = false;
        GoTo(ECharacterStates::Running, state_);
    }
    else if (physics_.velocity.y == GetGravity() * Engine::DeltaTime()) {
        //char_.doubleJump = false;
        GoTo(ECharacterStates::Idle, state_);
    }
    
}
DEFAULT_EXIT(ControllerFSM, InAir);


//attacking
void ControllerFSM::Attacking::Enter(ControllerFSM::StateComponent& state_) 
{
    auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
    ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Attacking, anim);
}
void ControllerFSM::Attacking::Run(ControllerFSM::StateComponent& state_)
{
    auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
    auto& char_ = Engine::Registry().get<BrawlerCharacter>(state_.entity);
    if (EPSILON_NOT_ZERO(char_.gotHit)) GoTo(ECharacterStates::Hitted, state_);
    else if (EPSILON_ZERO(char_.attacking))
    {
        char_.attacking = 0.4f;
    }
    else
    {
        char_.attacking -= Engine::DeltaTime();
        char_.hitSize = 100 * char_.attacking + 1;
        if (char_.attacking <= 0)
        {
            char_.attacking = 0.f;
            char_.hitSize = 100;
            char_.hittedEnemy = false;
            if (EPSILON_NOT_ZERO(input_.Get("jump"))) GoTo(ECharacterStates::InAir, state_);
            else if (EPSILON_NOT_ZERO(input_.Get("run"))) GoTo(ECharacterStates::Running, state_);
            else GoTo(ECharacterStates::Idle, state_);
        }
    }
}
DEFAULT_EXIT(ControllerFSM, Attacking);


//hitted
void ControllerFSM::Hitted::Enter(ControllerFSM::StateComponent& state_)
{
    auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
    ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Hitted, anim);
}
void ControllerFSM::Hitted::Run(ControllerFSM::StateComponent& state_)
{
    auto& char_ = Engine::Registry().get<BrawlerCharacter>(state_.entity);
    auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);

    char_.gotHit -= Engine::DeltaTime();

    if (char_.gotHit <= 0) {
        char_.gotHit = 0.f;
        if (char_.healthHearts == 0) { 
            GoTo(ECharacterStates::Dead, state_);
            char_.deaths++;

        }
        else if (input_.Get("light")) GoTo(ECharacterStates::Attacking, state_);
        else if (input_.Get("run"))GoTo(ECharacterStates::Running, state_);
        else GoTo(ECharacterStates::Idle, state_);
    }
}
DEFAULT_EXIT(ControllerFSM, Hitted);

//dead
void ControllerFSM::Dead::Enter(ControllerFSM::StateComponent& state_)
{
    auto& anim = Engine::Registry().get<AnimationsFSM::StateComponent>(state_.entity);
    ((ControllerFSM*)this->GetFSM())->animationsFSM.GoTo(EAnimationsState::Dead, anim);
}
void ControllerFSM::Dead::Run(ControllerFSM::StateComponent& state_)
{
    /*Engine::Registry().clear();
    team_game::SetupWorld_Demo(Engine::Instance());*/

    auto& char_ = Engine::Registry().get<BrawlerCharacter>(state_.entity);
    auto& sprite_ = Engine::Registry().get<Sprite>(state_.entity);
    auto& transform_ = Engine::Registry().get<Transform>(state_.entity);
    transform_.position = { 2550,26,0 };
    char_.healthHearts = 3;
    GoTo(ECharacterStates::Idle, state_);
    /*if (!char_.dead)
    {
        if (EPSILON_ZERO(char_.gotHit))
        {
            char_.gotHit = 0.7f;
        }
        else
        {
            char_.gotHit -= Engine::DeltaTime();
            if (char_.gotHit <= 0)
            {
                auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
                AnimatorStop(animator_);
            }

        }
    }*/
}
DEFAULT_EXIT(ControllerFSM, Dead);


//interact
DEFAULT_ENTER(ControllerFSM, Interact);
void ControllerFSM::Interact::Run(ControllerFSM::StateComponent& state_)
{
    auto& input_ = Engine::Registry().get<InputReceiver>(state_.entity);
    if (input_.Get("interact"))
    {
        auto& t_ = Engine::Registry().get<Transform>(state_.entity);
        if (t_.position.x > 2530 && t_.position.x<2575 && t_.position.y>-27 && t_.position.y < 26)
        {
            t_.position = { 2160, 1175, 0.0f };
            auto view = Engine::Registry().view<BrawlerCharacter>();
            for (auto entity : view)
            {
                //THIS IS IN CASE WE WANT THE CAMERA TO FOLLOW THE BOSS AND OUR CHARACTER
                //if(entity!=state_.entity)  Engine::Registry().emplace<team_game::CameraFollow>(entity);
                auto& brawler = Engine::Registry().get<BrawlerCharacter>(entity);
                brawler.inRoom = true;
            }
        }
        else if (EPSILON_NOT_ZERO(input_.Get("jump"))) GoTo(ECharacterStates::InAir, state_);
        else if (EPSILON_NOT_ZERO(input_.Get("run"))) GoTo(ECharacterStates::Running, state_);
        else GoTo(ECharacterStates::Idle, state_);
    }
    else if(EPSILON_NOT_ZERO(input_.Get("jump"))) GoTo(ECharacterStates::InAir, state_);
    else if (EPSILON_NOT_ZERO(input_.Get("run"))) GoTo(ECharacterStates::Running, state_);
    else GoTo(ECharacterStates::Idle, state_);

}
DEFAULT_EXIT(ControllerFSM, Interact);

