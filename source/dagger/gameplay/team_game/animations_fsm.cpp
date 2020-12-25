#include "animations_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "gameplay/team_game/brawler_controller.h"
#include "gameplay/team_game/physics.h"

using namespace dagger;
using namespace team_game;
//idle
DEFAULT_ENTER(AnimationsFSM, Idle);
void AnimationsFSM::Idle::Run(AnimationsFSM::StateComponent& state_) {
    auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator_, "character:IDLE");
}
DEFAULT_EXIT(AnimationsFSM, Idle);

//running
DEFAULT_ENTER(AnimationsFSM, Running);

void AnimationsFSM::Running::Run(AnimationsFSM::StateComponent& state_) {
    auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator_, "character:RUN");
}
DEFAULT_EXIT(AnimationsFSM, Running);

//jumping
DEFAULT_ENTER(AnimationsFSM, Jumping);
void AnimationsFSM::Jumping::Run(AnimationsFSM::StateComponent& state_)
{
    auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator_, "character:JUMP");
    auto& physics_ = Engine::Registry().get<Physics>(state_.entity);
    if (physics_.velocity.y < 0)
    {
        GoTo(EAnimationsState::Falling, state_);
    }
}
DEFAULT_EXIT(AnimationsFSM, Jumping);

//falling
DEFAULT_ENTER(AnimationsFSM, Falling);
void AnimationsFSM::Falling::Run(AnimationsFSM::StateComponent& state_)
{
    auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator_, "character:FALL");
    auto& physics_ = Engine::Registry().get<Physics>(state_.entity);
    if (physics_.velocity.y > 0)
    {
        GoTo(EAnimationsState::Jumping, state_);
    }
}
DEFAULT_EXIT(AnimationsFSM, Falling);

DEFAULT_ENTER(AnimationsFSM, Attacking);
void AnimationsFSM::Attacking::Run(AnimationsFSM::StateComponent& state_)
{
    auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator_, "character:ATTACK");
    

}
DEFAULT_EXIT(AnimationsFSM, Attacking);

//hitted
DEFAULT_ENTER(AnimationsFSM, Hitted);
void AnimationsFSM::Hitted::Run(AnimationsFSM::StateComponent& state_)
{
    auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator_, "character:HITTED");
}
DEFAULT_EXIT(AnimationsFSM, Hitted);

//dead
DEFAULT_ENTER(AnimationsFSM, Dead);
void AnimationsFSM::Dead::Run(AnimationsFSM::StateComponent& state_)
{
    auto& animator_ = Engine::Registry().get<Animator>(state_.entity);
    auto& sprite_ = Engine::Registry().get<Sprite>(state_.entity);
    AssignSprite(sprite_,"EmptyWhitePixel");
}
DEFAULT_EXIT(AnimationsFSM, Dead);