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
        else if (line.starts_with("#use vertex"))
            stages = stages | ShaderStage::Vertex;
        else if (line.starts_with("#use fragment"))
            stages = stages | ShaderStage::Fragment;
        else if (line.starts_with("#use compute"))
            stages = stages | ShaderStage::Compute;
        else if (line.starts_with("#use geometry"))
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
        if (entry.is_regular_file() && entry.path().string().ends_with(".shader"))
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
