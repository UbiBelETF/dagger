#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace team_game
{
    void SetupWorld(Engine& engine_);

    class TeamGame : public Game
    {
        inline String GetIniFile() override
        {
            return "teamgame.ini";
        };

        void GameplaySystemsSetup(Engine& engine_) override;
        void WorldSetup(Engine& engine_) override;
    };
}
