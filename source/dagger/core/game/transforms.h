#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

struct Transform
{
    Vector3 position{ 0, 0, 0 };

    static void PropertiesDrawer(Entity entity_);
};

class TransformSystem
    : public System
{
    inline String SystemName() { return "Transform System"; }

    void Run() override;
};