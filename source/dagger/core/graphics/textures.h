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
    static Texture* Get(String name_);

    Seq<UInt64> m_TextureHandles;

    void OnShaderChanged(ShaderChangeRequest request_);
    void OnLoadAsset(AssetLoadRequest<Texture> request_);
    void SpinUp() override;
    void WindDown() override;
};