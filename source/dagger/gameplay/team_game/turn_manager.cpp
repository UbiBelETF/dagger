#include "turn_manager.h"

#include "core/core.h"
#include "core/engine.h"
#include "gameplay/team_game/board.h"

using namespace dagger;

void TurnManagerFSM::GameStart::Enter(TurnManagerFSM::StateComponent& state_)
{}

void TurnManagerFSM::GameStart::Exit(TurnManagerFSM::StateComponent& state_)
{}

void TurnManagerFSM::GameStart::Run(TurnManagerFSM::StateComponent& state_)
{}



void TurnManagerFSM::TurnStart::Enter(TurnManagerFSM::StateComponent& state_)
{}

void TurnManagerFSM::TurnStart::Exit(TurnManagerFSM::StateComponent& state_)
{}

void TurnManagerFSM::TurnStart::Run(TurnManagerFSM::StateComponent& state_)
{}



void TurnManagerFSM::TimedMove::Enter(TurnManagerFSM::StateComponent& state_)
{}

void TurnManagerFSM::TimedMove::Exit(TurnManagerFSM::StateComponent& state_)
{}

void TurnManagerFSM::TimedMove::Run(TurnManagerFSM::StateComponent& state_)
{}



void TurnManagerFSM::TurnEnd::Enter(TurnManagerFSM::StateComponent& state_)
{}

void TurnManagerFSM::TurnEnd::Exit(TurnManagerFSM::StateComponent& state_)
{}

void TurnManagerFSM::TurnEnd::Run(TurnManagerFSM::StateComponent& state_)
{}



void TurnManagerFSM::GameEnd::Enter(TurnManagerFSM::StateComponent& state_)
{}

void TurnManagerFSM::GameEnd::Exit(TurnManagerFSM::StateComponent& state_)
{}

void TurnManagerFSM::GameEnd::Run(TurnManagerFSM::StateComponent& state_)
{}