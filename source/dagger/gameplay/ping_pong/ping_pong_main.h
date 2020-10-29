#pragma once

#include "core/core.h"
#include "core/system.h"

using namespace dagger;

namespace pingPong
{
    void SetupSystems(Engine& engine_);
    void SetupWorld(Engine& engine_);
    void CreatePingPongBall(entt::registry &reg_, float TileSize_, Color color_, Vector3 speed_, Vector3 pos_);
}