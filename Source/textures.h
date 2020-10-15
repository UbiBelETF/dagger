#pragma once

#include "system.h"
#include "structures.h"
#include "texture.h"

#include <glad.h>
#include <vector>

#include "shaders.h"

using namespace dagger;

struct TextureSystem 
    : public System
    , public Subscriber<ShaderChangeRequest, AssetLoadRequest<Texture>>
{
    static unsigned char Get(std::string name_);

    std::vector<std::uint64_t> m_TextureHandles;

    void OnShaderChanged(ShaderChangeRequest request_);
    void OnLoadAsset(AssetLoadRequest<Texture> request_);
    void SpinUp() override;
    void WindDown() override;
};