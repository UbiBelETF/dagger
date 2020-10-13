#pragma once

#include "system.h"
#include "structures.h"
#include "texture.h"

#include <glad.h>
#include <vector>

using namespace dagger;

struct TextureUploadRequest
{
    unsigned int m_ShaderId;
    unsigned int m_UniformPosition;
};

struct TextureSystem 
    : public System
{
    static unsigned char Get(std::string name_);

    std::vector<std::uint64_t> m_TextureHandles;

    void OnLoadAsset(AssetLoadRequest request_);
    void OnRequireTextureUpload(TextureUploadRequest request_);
    void SpinUp() override;
    void WindDown() override;
};