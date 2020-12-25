#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

#include <list>

using namespace dagger;

namespace ancient_defenders
{
    void SetupWorld(Engine& engine_);
    void SetupControls(Engine& engine_);
    void LoadPath();
    void LoadTowerSpots();

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
