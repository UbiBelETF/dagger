#pragma once

#include "texture.h"
#include "shaders.h"
#include "core/system.h"
#include "core/core.h"

#include "tools/toolmenu.h"

#include <glad/glad.h>

using namespace dagger;

struct TextureSystem 
    : public System
    , public Subscriber<ShaderChangeRequest, AssetLoadRequest<Texture>>
    , public Publisher<ToolMenuRender>
{
    inline String SystemName() { return "Texture System"; }

    static ViewPtr<Texture> Get(String name_);

    Sequence<UInt64> m_TextureHandles;

    void RenderToolMenu();

    void LoadDefaultAssets();

    void OnLoadAsset(AssetLoadRequest<Texture> request_);
    void SpinUp() override;
    void WindDown() override;
};