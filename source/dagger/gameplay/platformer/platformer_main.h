#pragma once

#include "core/game.h"
#include "core/core.h"


#define HEALTH_BAR_START_SIZE 50.f

using namespace dagger;

namespace platformer
{  

    class Platformer : public Game
    {
        String GetIniFile() override
        {
            return "platformer.ini";
        }

        void GameplaySystemsSetup(Engine& engine_) override;
        void WorldSetup(Engine& engine_) override;
    };
}