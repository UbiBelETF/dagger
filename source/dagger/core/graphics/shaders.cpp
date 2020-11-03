#include "core/graphics/shaders.h"
#include "core/engine.h"
#include "core/stringops.h"
#include "core/filesystem.h"
#include <string>
#include <regex>

using namespace dagger;

void ShaderSystem::Use(String name_)
{
    auto shader = Engine::Res<Shader>()[name_];
    assert(shader != nullptr);
    glUseProgram(shader->programId);
    Engine::Dispatcher().trigger<ShaderChangeRequest>(ShaderChangeRequest(shader));
}

UInt32 ShaderSystem::Get(String name_)
{
    auto shader = Engine::Res<Shader>()[name_];
    assert(shader != nullptr);
    return shader->programId;
}

void ShaderSystem::OnLoadAsset(AssetLoadRequest<Shader> request_)
{
    FilePath path(request_.path);

    if (!Files::exists(path))
    {
        Engine::Dispatcher().trigger<Error>(Error{ fmt::format("Couldn't load shader from {}.", request_.path) });
        return;
    }

    FileInputStream handle;
    auto absolutePath = Files::absolute(path);
    handle.open(absolutePath);

    if (!handle.is_open())
    {
        Engine::Dispatcher().trigger<Error>(Error{ fmt::format("Couldn't open shader file '{}' for reading.", absolutePath.string()) });
        return;
    }

    
    JSON::json json;
    handle >> json;

    ShaderConfig config;

    if (json.contains("program-name"))
    {
        config.name = json["program-name"];
    }

    if (json.contains("shader-stages"))
    {
        Map<EShaderStage, String> stageLoader;

        auto stages = json["shader-stages"];
        for (auto [key, value] : stages.get<JSON::json::object_t>())
        {
            assert(Shader::s_ShaderStageIndex.contains(key));
            auto stage = Shader::s_ShaderStageIndex[key];
            config.stages = config.stages | stage;
            stageLoader[stage] = stages[key];
        }

        config.paths = stageLoader;
        auto shader = new Shader(config);
        Engine::Res<Shader>()[config.name] = shader;
    }
}

void ShaderSystem::SpinUp()
{
    Engine::Dispatcher().sink<AssetLoadRequest<Shader>>().connect<&ShaderSystem::OnLoadAsset>(this);

    for (auto& entry : Files::recursive_directory_iterator("shaders"))
    {
        auto path = entry.path().string();
        if (entry.is_regular_file() && entry.path().extension() == ".json")
        {
            Engine::Dispatcher().trigger<AssetLoadRequest<Shader>>(AssetLoadRequest<Shader>{ path });
        }
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
