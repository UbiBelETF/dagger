#pragma once

#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/system.h"
#include "core/core.h"
#include "gameplay/team_game/animation_fsm.h"

#if !defined(NDEBUG)
#include "tools/toolmenu.h"
#endif //!defined(NDEBUG)

#include <glad/glad.h>

using namespace dagger;
using namespace team_game;

class AnimationSystem
    : public System
    , public Subscriber<AssetLoadRequest<Animation>>
#if !defined(NDEBUG)
    , public Publisher<ToolMenuRender>
#endif //!defined(NDEBUG)
{
public:
    inline String SystemName() { return "Animation System"; }

    static ViewPtr<Animation> Get(String name_);

#if !defined(NDEBUG)
    void RenderToolMenu();
#endif //!defined(NDEBUG)

    void LoadDefaultAssets();
    void OnLoadAsset(AssetLoadRequest<Animation> request_);
    void SpinUp() override;
    void Run() override;
    void WindDown() override;
};