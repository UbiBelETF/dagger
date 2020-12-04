#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace team_game
{
    class GameManagerSystem : public System
    {
    private:
        inline static Sequence<Vector3> playerPositionsPerLevel{};

        inline static UInt8 currentLevel{ 0 };
        inline static Bool completedObjective{ true };

        void OnEndOfFrame();
    public:
        inline String SystemName() { return " Game Manager System "; }

        void SpinUp() override;
        void WindDown() override;

        void Run() override;

        void LoadNextLevel();
        void LoadTextures(String filePath_, Bool addCollision_);
        void LoadBackDrop();
        void LoadPlatforms();
        void LoadTraps();

        static inline Sequence<Vector3>& GetPlayerPositionsPerLevel()
        {
            return playerPositionsPerLevel;
        }

        static inline UInt8 GetCurrentLevel()
        {
            return currentLevel;
        }

        static inline Bool& IsObjectiveCompleted()
        {
            return completedObjective;
        }
    };
};
