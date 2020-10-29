#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace pingPong
{
    void SetupSystems(Engine& engine_);
    void SetupWorld(Engine& engine_);
    void CreatePingPongBall(entt::registry &reg, float TileSize, Color color, Vector3 speed, Vector3 pos);
}