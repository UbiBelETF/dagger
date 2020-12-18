#pragma once
#include "core/core.h"
#include "core/game/finite_state_machine.h"

enum struct EBossState {
    Idle,
    Running,
    Attack
};

class BossFSM : public FSM<EBossState>
{
public:
    DEFINE_STATE(BossFSM, EBossState, Idle);
    DEFINE_STATE(BossFSM, EBossState, Running);
    DEFINE_STATE(BossFSM, EBossState, Attack);
    

    BossFSM() {
        CONNECT_STATE(EBossState, Idle);
        CONNECT_STATE(EBossState, Running);
        CONNECT_STATE(EBossState, Attack);
    }

};
