#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct AnimationsState
{
    Idle,
    Running,
    Falling,
    Jumping
};


struct AnimationsFSM :public FSM<AnimationsState>
{
    DEFINE_STATE(AnimationsFSM,AnimationsState, Idle);
    DEFINE_STATE(AnimationsFSM,AnimationsState, Running);
    DEFINE_STATE(AnimationsFSM,AnimationsState, Jumping);
    DEFINE_STATE(AnimationsFSM,AnimationsState, Falling);

    AnimationsFSM() {
        CONNECT_STATE(AnimationsState, Idle);
        CONNECT_STATE(AnimationsState, Running);
        CONNECT_STATE(AnimationsState, Jumping);
        CONNECT_STATE(AnimationsState, Falling);
    }
    

};
