#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace tank_warfare
{
    void SetupCamera(Engine& engine_, UInt32 zoom_ = 1);
    void SetupStartScreen(Engine& engine_);
    void SetupRestartScreen(Engine& engine_, String winner_);
    void SetupWorld(Engine& engine_);
    void SetupTestWorld(Engine& engine_);

    class TankWarfare : public Game
    {
        inline String GetIniFile() override
        {
            return "tankwarfare.ini";
        };

        void GameplaySystemsSetup(Engine& engine_) override;
        void WorldSetup(Engine& engine_) override;
    };
}
