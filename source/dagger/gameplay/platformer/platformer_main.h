#pragma once

#include "core/core.h"
#include "core/game.h"

using namespace dagger;

class Platformer : public Game
{
    String GetIniFile() override
    {
        return "platformer.ini";
    }

    void GameplaySystemsSetup(Engine& engine_) override;
    void WorldSetup(Engine& engine_) override;
};
