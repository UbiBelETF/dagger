#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace lomm
{
    struct Ground
    {
        int tile;
    };

    struct Water
    {
        int depth;
    };

    class LommGame : public Game
    {
        inline String GetIniFile() override
        {
            return "lomm.ini";
        };

        void GameplaySystemsSetup(Engine& engine_) override;
        void WorldSetup(Engine& engine_) override;
    };
}
