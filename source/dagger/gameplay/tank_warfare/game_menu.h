#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace tank_warfare
{
    enum struct EGameMenuType
    {
        StartScreen,
        RestartScreen
    };

    struct GameMenuButton
    {
        EGameMenuType type{ EGameMenuType::StartScreen };
        Vector2 size{ 0, 0 };
        Vector3 position{ 0, 0, 0 };
    };

    class GameMenuSystem
        : public System
    {
        bool m_LoadGame = false;
        static bool s_GameOver;
        static String s_Winner;

    public:
        inline String SystemName() { return "Game Menu System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

        bool IsMouseOver(GameMenuButton gmb_);
        static void EndOfGame(String winner_);

    private:
        void OnEndOfFrame();

    };
}
