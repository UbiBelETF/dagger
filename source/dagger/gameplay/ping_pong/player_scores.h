#pragma once
#include "core/system.h"
#include "core/core.h"

#include "gameplay/ping_pong/pingpong_ball.h"

using namespace dagger;

namespace ping_pong
{
    class PlayerScoresSystem : public System
    {
        static int s_FieldWidth;
        static int s_FieldHeight;
        static Float32 s_TileSize;

        int m_goalsPlayerOne = 0;
        int m_goalsPlayerTwo = 0;

    public:
        inline String SystemName() { return "Player Scores System"; }

        void Run() override;


        static Float32 GetTileSize() { return s_TileSize; }
        static void SetFieldSize(int width, int height, float tileSize)
        {
            s_FieldWidth = width;
            s_FieldHeight = height;
            s_TileSize = tileSize;
        }
    };
}