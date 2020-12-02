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
        inline static UInt8 currentLevel{ 0 };
        inline static Bool completedObjective{ true };

        inline String SystemName() { return " Game Manager System "; }

        void SpinUp() override;
        void WindDown() override;

        void Run() override;

        void LoadNextLevel();
        void LoadTextures(String filePath_, Bool addCollision_);
        void LoadBackDrop();
        void LoadPlatforms();
        void LoadTraps();
    
    private:
        void OnEndOfFrame();
    };
};
