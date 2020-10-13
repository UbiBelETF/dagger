#include "textures.h"
#include "engine.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include "loaders/stb_image.h"

#include <filesystem>

using namespace dagger;

unsigned char TextureSystem::Get(std::string name_)
{
    auto texture = Engine::Res<Texture>()[name_];
    assert(texture != nullptr);
    return texture->Index();
}

void TextureSystem::OnLoadAsset(AssetLoadRequest request_)
{
    if (request_.m_Path.ends_with(".png"))
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
}

void TextureSystem::OnRequireTextureUpload(TextureUploadRequest request_)
{
    glProgramUniformHandleui64vARB(request_.m_ShaderId, request_.m_UniformPosition, 
        (GLsizei)m_TextureHandles.size(), m_TextureHandles.data());
}

void TextureSystem::SpinUp()
{
    Engine::Dispatcher().sink<AssetLoadRequest>().connect<&TextureSystem::OnLoadAsset>(this);
    Engine::Dispatcher().sink<TextureUploadRequest>().connect<&TextureSystem::OnRequireTextureUpload>(this);

    for (const auto& entry : std::filesystem::directory_iterator("Res\\Textures"))
    {
        if(entry.is_regular_file() && entry.path().string().ends_with(".png"))
            Engine::Dispatcher().trigger<AssetLoadRequest>(AssetLoadRequest(entry.path().string()));
    }
}

void TextureSystem::WindDown()
{
    auto& textures = Engine::Res<Texture>();
    for (auto t = textures.begin(); t != textures.end(); t++)
    {
        delete t->second;
    }

    Engine::Dispatcher().sink<AssetLoadRequest>().disconnect<&TextureSystem::OnLoadAsset>(this);
    Engine::Dispatcher().sink<TextureUploadRequest>().disconnect<&TextureSystem::OnRequireTextureUpload>(this);
}
