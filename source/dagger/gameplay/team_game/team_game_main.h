#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace ancient_defenders
{
    void SetupWorld(Engine& engine_);
    void SetupControls(Engine& engine_);
    void LoadPath();
    void LoadTowerSpots();

    void SetupStartScreen(Engine& engine_);
    void SetupEndScreen(Engine& engine_, Bool goodEnd = true);

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
