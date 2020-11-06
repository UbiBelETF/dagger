#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace platformer
{
    struct Parallax
    {
        Vector2 lastCameraPosition{ 0, 0 };
        Float32 strength{ 0 };
    };

    class ParallaxSystem
        : public System
    {
        inline String SystemName() override {
            return "Parallax System";
        };

        void Run() override;
    };
}