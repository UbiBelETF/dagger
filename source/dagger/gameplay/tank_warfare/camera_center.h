#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace tank_warfare
{
    struct CameraCenter
    {
        UInt32 weight{ 1 };
    };

    struct CameraParams
    {
        Float32 camZoom{ 1 };
        Vector2 camXY{ 800, 600 };
    };

    class CameraCenterSystem
        : public System
    {
    public:

        inline String SystemName() override {
            return "Camera Center System";
        };

        void Run() override;
    };
}