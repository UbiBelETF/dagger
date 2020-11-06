#pragma once

#include "core/core.h"

namespace dagger
{
    class Engine;

    struct Game
    {
        virtual void CoreSystemsSetup(Engine& engine_);

        virtual String GetIniFile() = 0;
        virtual void GameplaySystemsSetup(Engine& engine_) = 0;
        virtual void WorldSetup(Engine& engine_) = 0;
    };
}
