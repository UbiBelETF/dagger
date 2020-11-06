#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace platformer
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
    };
}