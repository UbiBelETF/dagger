#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace racing_game
{
    void CreateStrike(float tileSize_,int lifePoints);
    struct RacingGameFieldSettings
    {
        int fieldWidth;
        int fieldHeight;
        Float32 fieldTileSize;

        Float32 GetXBoarder()
        {
            return (fieldWidth - 2) / 2 * fieldTileSize;
        }

        Float32 GetYBoarder()
        {
            return (fieldHeight) / 2 * fieldTileSize;
        }
    };

    struct RacingGameStats
    {
        int scores = 0;
        
    };

    class RacingCollisionsLogicSystem
        : public System
    {
        bool m_Restart = false;

    public:
        inline String SystemName() { return "Racing Collision Car System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        void OnEndOfFrame();

    };
}
