#pragma once

#include "texture.h"
#include "shaders.h"
#include "core/system.h"
#include "core/core.h"

#include <glad/glad.h>

using namespace dagger;

struct TextureSystem 
    : public System
    , public Subscriber<ShaderChangeRequest, AssetLoadRequest<Texture>>
{
    inline String SystemName() { return "Texture System"; }

    static ViewPtr<Texture> Get(String name_);

    Sequence<UInt64> m_TextureHandles;

    void OnShaderChanged(ShaderChangeRequest request_);
    void OnLoadAsset(AssetLoadRequest<Texture> request_);
    void SpinUp() override;
    void WindDown() override;
};