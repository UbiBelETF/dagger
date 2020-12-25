#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct EBossState {
    Idle,
    Running,
    Attack,
    Hitted,
    Dead
};

class BossFSM : public FSM<EBossState>
{
public:
    DEFINE_STATE(BossFSM, EBossState, Idle);
    DEFINE_STATE(BossFSM, EBossState, Running);
    DEFINE_STATE(BossFSM, EBossState, Attack);
    DEFINE_STATE(BossFSM, EBossState, Hitted);
    DEFINE_STATE(BossFSM, EBossState, Dead);

    BossFSM() {
        CONNECT_STATE(EBossState, Idle);
        CONNECT_STATE(EBossState, Running);
        CONNECT_STATE(EBossState, Attack);
        CONNECT_STATE(EBossState, Hitted);
        CONNECT_STATE(EBossState, Dead);
    }

};
