#pragma once

#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/system.h"
#include "core/core.h"

#if !defined(NDEBUG)
#include "tools/toolmenu.h"
#endif //!defined(NDEBUG)

#include <glad/glad.h>

using namespace dagger;

class AnimationSystem
    : public System
    , public Subscriber<AssetLoadRequest<Animation>, ToolMenuRender>
{

public:
    inline String SystemName() { return "Animation System"; }

    static ViewPtr<Animation> Get(String name_);

    void OnToolMenuRender();

    void LoadDefaultAssets();
    void OnLoadAsset(AssetLoadRequest<Animation> request_);
    void SpinUp() override;
    void Run() override;
    void WindDown() override;
};