#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace plight
{
    void SetupWorld(Engine& engine_);
    void ResetCharacters();
    void SetupWorld_test1(Engine& engine_);
    void SetupWorld_CombatSystem(Engine& engine_);
    void SetupWorld_AimingSystem(Engine& engine_);
    void SetupTilemaps();
	void SetupMainManu(Engine& engine_);


    class Plight : public Game
    {
        inline String GetIniFile() override
        {
            return "plight.ini";
        };

        void GameplaySystemsSetup(Engine& engine_) override;
        void WorldSetup(Engine& engine_) override;
    };
}
