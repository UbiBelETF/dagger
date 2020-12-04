#pragma once
#include "core/core.h"
#include "core/system.h"
#include "core/game.h"
#include "core/engine.h"
#include <functional>
#include "tilemap.h"

using namespace dagger;


class TilemapSystem : public System,public Subscriber<TilemapLoadRequest>
{

    void SpinUp() override;
    void WindDown() override;

    void OnLoadAsset(TilemapLoadRequest request_);

};
