#include "textures.h"
#include "engine.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb/stb_image.h"

#include <filesystem>

using namespace dagger;

Texture* TextureSystem::Get(String name_)
{
    auto texture = Engine::Res<Texture>()[name_];
    assert(texture != nullptr);
    return texture;
}

void TextureSystem::OnLoadAsset(AssetLoadRequest<Texture> request_)
{
    spdlog::info("Loading texture {}...", request_.m_Path);
    stbi_set_flip_vertically_on_load(true);

    const FilePath path{ request_.m_Path };
    const String textureName = path.stem().string();

    // these have to remain "int" because of API/ABI-compatibility with stbi_load
    int width, height, channels;
    UInt8* image = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
    spdlog::info("Image statistics: name ({}), width ({}), height ({}), depth ({})", textureName, width, height, channels);

    if (image == nullptr)
    {
        Engine::Dispatcher().trigger<Error>(Error(fmt::format("Failed to load texture: {}", path.string())));
        return;
    }

    auto* texture = new Texture(textureName, path, image, (UInt32)width, (UInt32)height, (UInt32)channels);
    UInt32 currentIndex = (UInt32) m_TextureHandles.size();
    m_TextureHandles.push_back(texture->Handle());

    texture->m_TextureIndex = currentIndex;
    assert(width != 0);
    texture->m_Ratio = (Float32)height / (Float32)width;
    assert(texture->m_Ratio != 0);

    Engine::Res<Texture>()[textureName] = texture;
    spdlog::info("Texture index: {}", texture->m_TextureIndex);
    stbi_image_free(image);
}

void TextureSystem::OnShaderChanged(ShaderChangeRequest request_)
{
    spdlog::info("Shader changed to {}, reuploading textures.", request_.m_Shader->m_ShaderName);
    glProgramUniformHandleui64vARB(request_.m_Shader->m_ProgramId, Shader::TextureBufferId(),
        (GLsizei)m_TextureHandles.size(), m_TextureHandles.data());
}

void TextureSystem::SpinUp()
{
    Engine::Dispatcher().sink<ShaderChangeRequest>().connect<&TextureSystem::OnShaderChanged>(this);
    Engine::Dispatcher().sink<AssetLoadRequest<Texture>>().connect<&TextureSystem::OnLoadAsset>(this);

    for (const auto& entry : std::filesystem::directory_iterator("Res\\Textures"))
    {
        if(entry.is_regular_file() && entry.path().string().ends_with(".png"))
            Engine::Dispatcher().trigger<AssetLoadRequest<Texture>>(AssetLoadRequest<Texture>(entry.path().string()));
        else
        {
            Engine::Dispatcher().trigger<Error>(
                Error(fmt::format("Cannot read texture from {}, only PNG (4 channel) images supported.", entry.path().string())));
            return;
        }
    }
}

void TextureSystem::WindDown()
{
    auto& textures = Engine::Res<Texture>();
    for (auto t = textures.begin(); t != textures.end(); t++)
    {
        delete t->second;
    }

    Engine::Dispatcher().sink<AssetLoadRequest<Texture>>().disconnect<&TextureSystem::OnLoadAsset>(this);
    Engine::Dispatcher().sink<ShaderChangeRequest>().disconnect<&TextureSystem::OnShaderChanged>(this);
}
