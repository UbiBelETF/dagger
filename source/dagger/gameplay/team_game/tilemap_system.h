#pragma once
#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include <functional>
#include "tilemap.h"

using namespace dagger;

namespace team_game {
    class TilemapSystem : public System, public Subscriber<TilemapLoadRequest>
    {
    public:
        String SystemName() override {
            return "Team Game Tilemap System";
        }
        void SpinUp() override;
        void Run() override;
        void WindDown() override;

        void OnLoadAsset(TilemapLoadRequest request_);

    };
}
