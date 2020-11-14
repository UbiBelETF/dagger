#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"

using namespace dagger;

namespace ping_pong
{
    void CreatePingPongBall(float tileSize_, ColorRGBA color_, Vector3 speed_, Vector3 pos_);
    void SetupWorld(Engine& engine_);

    class PingPongGame : public Game
    {
        inline String GetIniFile() override
        {
            return "pingpong.ini";
        };

        void CoreSystemsSetup(Engine& engine_) override;
        void GameplaySystemsSetup(Engine& engine_) override;
        void WorldSetup(Engine& engine_) override;
    };
}
