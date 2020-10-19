#pragma once

#include "system.h"
#include "core.h"
#include "texture.h"
#include "shaders.h"

#include <glad.h>

using namespace dagger;

struct TextureSystem 
    : public System
    , public Subscriber<ShaderChangeRequest, AssetLoadRequest<Texture>>
{
    static Texture* Get(String name_);

    Vector<UInt64> m_TextureHandles;

    void OnShaderChanged(ShaderChangeRequest request_);
    void OnLoadAsset(AssetLoadRequest<Texture> request_);
    void SpinUp() override;
    void WindDown() override;
};