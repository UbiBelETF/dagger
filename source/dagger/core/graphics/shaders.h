#pragma once

#include "shader.h"
#include "core/system.h"
#include "core/core.h"

#include <glad/glad.h>
#include <vector>
#include <filesystem>

using namespace dagger;

struct ShaderChangeRequest
{
    Shader* m_Shader;

    explicit ShaderChangeRequest(Shader* shader_)
        : m_Shader{ shader_ }
    {}
};

struct ShaderSystem
    : public System
    , public Subscriber<AssetLoadRequest<Shader>>
{
    static void Use(String name_);
    static UInt32 Get(String name_);

    void OnLoadAsset(AssetLoadRequest<Shader> request_);
    void SpinUp() override;
    void WindDown() override;
};