#include "shaders.h"
#include "core/engine.h"
#include "core/stringops.h"

#include <filesystem>
#include <fstream>
#include <string>
#include <regex>

using namespace dagger;

void ShaderSystem::Use(String name_)
{
    auto shader = Engine::Res<Shader>()[name_];
    assert(shader != nullptr);
    glUseProgram(shader->m_ProgramId);
    Engine::Dispatcher().trigger<ShaderChangeRequest>(ShaderChangeRequest(shader));
}

UInt32 ShaderSystem::Get(String name_)
{
    auto shader = Engine::Res<Shader>()[name_];
    assert(shader != nullptr);
    return shader->m_ProgramId;
}

void ShaderSystem::OnLoadAsset(AssetLoadRequest<Shader> request_)
{
    FilePath path(request_.m_Path);

    if (!Files::exists(path))
    {
        Engine::Dispatcher().trigger<Error>(Error(fmt::format("Couldn't load shader from {}.", request_.m_Path)));
        return;
    }

    std::ifstream handle;
    auto absolutePath = Files::absolute(path);
    handle.open(absolutePath);

    if (!handle.is_open())
    {
        Engine::Dispatcher().trigger<Error>(Error(fmt::format("Couldn't open shader file '{}' for reading.", absolutePath.string())));
        return;
    }

    String line;
    ShaderStage stages{ ShaderStage::None };

    while (std::getline(handle, line))
    {
        line = trim(line);
        if (line.empty()) continue;
        else if (line.rfind("#use vertex", 0) == 0)
            stages = stages | ShaderStage::Vertex;
        else if (line.rfind("#use fragment", 0) == 0)
            stages = stages | ShaderStage::Fragment;
        else if (line.rfind("#use compute", 0) == 0)
            stages = stages | ShaderStage::Compute;
        else if (line.rfind("#use geometry", 0) == 0)
            stages = stages | ShaderStage::Geometry;
    }
    
    auto name = path.stem().string();
    auto shader = new Shader(name, stages);
    Engine::Res<Shader>()[name] = shader;
}

void ShaderSystem::SpinUp()
{
    Engine::Dispatcher().sink<AssetLoadRequest<Shader>>().connect<&ShaderSystem::OnLoadAsset>(this);

    for (const auto& entry : Files::directory_iterator("shaders"))
    {
        // TODO:: will work with .shader.something path
        if (entry.is_regular_file() && entry.path().string().rfind(".shader") != std::string::npos)
            Engine::Dispatcher().trigger<AssetLoadRequest<Shader>>(AssetLoadRequest<Shader>(entry.path().string()));
    }
}

void ShaderSystem::WindDown()
{
    auto& textures = Engine::Res<Shader>();
    for (auto t = textures.begin(); t != textures.end(); t++)
    {
        delete t->second;
    }

    Engine::Dispatcher().sink<AssetLoadRequest<Shader>>().disconnect<&ShaderSystem::OnLoadAsset>(this);
}
