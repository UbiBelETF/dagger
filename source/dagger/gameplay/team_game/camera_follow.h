#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/game/transforms.h"

using namespace dagger;

namespace team_game
{
    struct CameraFollowFocus
    {
        UInt32 weight{ 1 };
    };

    class CameraFollowSystem
        : public System
    {
        inline String SystemName() override {
            return "Camera Follow System";
        };

        void Run() override;

        void DistributeCameraWeight();

        void AdjustCameraZoom();

        Float32 CalculateDistance(Transform& transformOne_, Transform& transfromTwo_);
    };
}
