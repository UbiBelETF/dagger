#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/team_game/treasure.h"
using namespace dagger;

namespace team_game
{
    class GameManagerSystem : public System
    {
    private:
        inline static Sequence<Vector3> playerPositionsPerLevel{};

        inline static UInt8 winnerId{ 0 };
        inline static Bool isGameOver{ false };

        inline static UInt8 currentLevel{ 0 };
        inline static Bool gameStarted{ true };

        inline static Bool messageDisplayed{ false };

        void OnEndOfFrame();
    public:
        Bool restarted{ false };
        EDaggerKeyboard nextLevelKey{ EDaggerKeyboard::KeyY };

        inline String SystemName() { return " Game Manager System "; }

        void SpinUp() override;
        void WindDown() override;

        void Run() override;

        void OnKeyboardEvent(KeyboardEvent kEvent_);

        void LoadNextLevel();

        void LoadTextures(String filePath_, Bool addCollision_, Bool isTrap_ = false);
        void LoadBackDrop();
        void LoadPlatforms();
        void LoadTraps();
        void LoadCollectables();

        inline static Sequence<Vector3>& GetPlayerPositionsPerLevel()
        {
            return playerPositionsPerLevel;
        }

        inline static UInt8 GetCurrentLevel()
        {
            return currentLevel;
        }

        inline static void SetCurrentLevel(UInt8 newLevel_)
        {
            currentLevel = newLevel_;
        }
    };

    void SaveOnBoard(Entity entity_, Float32 position_);
};
