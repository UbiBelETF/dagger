#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace team_game
{
    struct ComponentOfLevel
    {
        Bool discard{ false };
    };

    struct Level
    {
        UInt8 level{ 0 };
        Bool completedObjective{ true };
    };

    class GameManagerSystem : public System
    {
    public:

        inline String SystemName() { return " Game Manager System "; }

        void SpinUp() override;
        void WindDown() override;

        void Run() override;

        void LoadNextLevel(Level& level_);
        void LoadBackDrop(UInt8 level_);
        void LoadPlatforms(UInt8 level_);
    
    private:
        void OnEndOfFrame();
    };
};
