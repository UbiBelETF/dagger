#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct EAnimationsState
{
    Idle,
    Running,
    Falling,
    Jumping,
    Attacking,
    Hitted,
    Dead
};


struct AnimationsFSM :public FSM<EAnimationsState>
{
    DEFINE_STATE(AnimationsFSM,EAnimationsState, Idle);
    DEFINE_STATE(AnimationsFSM,EAnimationsState, Running);
    DEFINE_STATE(AnimationsFSM,EAnimationsState, Jumping);
    DEFINE_STATE(AnimationsFSM,EAnimationsState, Falling);
    DEFINE_STATE(AnimationsFSM, EAnimationsState, Attacking);
    DEFINE_STATE(AnimationsFSM, EAnimationsState, Hitted);
    DEFINE_STATE(AnimationsFSM, EAnimationsState, Dead);

    AnimationsFSM() {
        CONNECT_STATE(EAnimationsState, Idle);
        CONNECT_STATE(EAnimationsState, Running);
        CONNECT_STATE(EAnimationsState, Jumping);
        CONNECT_STATE(EAnimationsState, Falling);
        CONNECT_STATE(EAnimationsState, Attacking);
        CONNECT_STATE(EAnimationsState, Hitted);
        CONNECT_STATE(EAnimationsState, Dead);
       
    }
    

};
