#pragma once

#include "core/system.h"

using namespace dagger;

class CameraSystem : public System
{
    inline String SystemName() override {
        return "Camera System";
    };

    void Run() override;
};
