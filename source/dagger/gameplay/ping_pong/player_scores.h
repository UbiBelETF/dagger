#pragma once
#include "core/system.h"
#include "core/core.h"

#include "gameplay/ping_pong/pingpong_ball.h"

using namespace dagger;

namespace pingPong
{

    class PlayerScoresSystem : public System
    {
        static int FIELD_WIDTH;
        static int FIELD_HEIGHT;
        static Float32 TILE_SIZE;

        int m_goalsPlayerOne = 0;
        int m_goalsPlayerTwo = 0;

    public:
        inline String SystemName() { return "Player Scores System"; }

        void Run() override;

        static void SetFieldSize(int width, int height, float tileSize)
        {
            FIELD_WIDTH = width;
            FIELD_HEIGHT = height;
            TILE_SIZE = tileSize;
        }
    };

}