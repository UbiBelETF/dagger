#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace team_game
{
    struct TeamCameraFollowFocus
    {
        UInt32 weight{ 1 };
    };

    class TeamCameraFollowSystem
        : public System
    {
        const float cameraFollowSpeed = 1.5f;

        inline String SystemName() override {
            return "Camera Follow System";
        };

        void Run() override;
    };
}