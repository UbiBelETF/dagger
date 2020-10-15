#include "textures.h"
#include "engine.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "stb/stb_image.h"

#include <filesystem>

using namespace dagger;

unsigned char TextureSystem::Get(std::string name_)
{
    auto texture = Engine::Res<Texture>()[name_];
    assert(texture != nullptr);
    return texture->Index();
}

void TextureSystem::OnLoadAsset(AssetLoadRequest<Texture> request_)
{
    spdlog::info("Loading texture {}...", request_.m_Path);
    stbi_set_flip_vertically_on_load(true);

    const std::filesystem::path path{ request_.m_Path };
    const std::string textureName = path.stem().string();

    int width, height, channels;
    unsigned char* image = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
    spdlog::info("Image statistics: name ({}), width ({}), height ({}), depth ({})", textureName, width, height, channels);

    if (image == nullptr)
    {
        Engine::Dispatcher().trigger<Error>(Error(fmt::format("Failed to load texture: {}", path.string())));
        return;
    }

    auto* texture = new Texture(textureName, path, image, width, height, channels);
    GLuint currentIndex = (GLuint) m_TextureHandles.size();
    m_TextureHandles.push_back(texture->Handle());
    texture->m_TextureIndex = currentIndex;
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
