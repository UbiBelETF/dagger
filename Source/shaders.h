#pragma once

#include "system.h"
#include "structures.h"
#include "shader.h"

#include <glad.h>
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
    static void Use(std::string name_);
    static unsigned int Get(std::string name_);

    void OnLoadAsset(AssetLoadRequest<Shader> request_);
    void SpinUp() override;
    void WindDown() override;
};