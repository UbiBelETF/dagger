#pragma once

#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/system.h"
#include "core/core.h"

#include "tools/toolmenu.h"

#include <glad/glad.h>

using namespace dagger;

class AnimationSystem
    : public System
    , public Subscriber<AssetLoadRequest<Animation>>
    , public Publisher<ToolMenuRender>
{

public:
    inline String SystemName() { return "Animation System"; }

    static ViewPtr<Animation> Get(String name_);

    void RenderToolMenu();

    void LoadDefaultAssets();
    void OnLoadAsset(AssetLoadRequest<Animation> request_);
    void SpinUp() override;
    void Run() override;
    void WindDown() override;
};