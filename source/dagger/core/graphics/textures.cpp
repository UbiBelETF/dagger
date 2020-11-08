#include "textures.h"
#include "core/engine.h"
#include "core/filesystem.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif
#include <stb/stb_image.h>

using namespace dagger;

ViewPtr<Texture> TextureSystem::Get(String name_)
{
    auto texture = Engine::Res<Texture>()[name_];
    assert(texture != nullptr);
    return texture;
}

void TextureSystem::OnLoadAsset(AssetLoadRequest<Texture> request_)
{
    Logger::info("Loading texture {}...", request_.path);
    stbi_set_flip_vertically_on_load(true);

    const FilePath path{ request_.path };
    String textureName = "";
    {
        String pathName = path.string();
        pathName = pathName.substr(9, pathName.length() - 13);
        std::replace(pathName.begin(), pathName.end(), '/', ':');
        std::replace(pathName.begin(), pathName.end(), '\\', ':');
        textureName = pathName;
    }

    // these have to remain "int" because of API/ABI-compatibility with stbi_load
    int width, height, channels;
    UInt8* image = stbi_load(path.string().c_str(), &width, &height, &channels, 0);
    Logger::info("Image statistics: name ({}), width ({}), height ({}), depth ({})", textureName, width, height, channels);

    if (image == nullptr)
    {
        Engine::Dispatcher().trigger<Error>(Error{ fmt::format("Failed to load texture: {}", path.string()) });
        return;
    }

    auto* texture = new Texture(textureName, path, image, (UInt32)width, (UInt32)height, (UInt32)channels);
    UInt32 currentIndex = (UInt32) m_TextureHandles.size();

    assert(width != 0);
    texture->m_Ratio = (Float32)height / (Float32)width;
    assert(texture->m_Ratio != 0);

    Engine::Res<Texture>()[textureName] = texture;
    stbi_image_free(image);
}

void TextureSystem::SpinUp()
{
    Engine::Dispatcher().sink<AssetLoadRequest<Texture>>().connect<&TextureSystem::OnLoadAsset>(this);

    for (auto& entry : Files::recursive_directory_iterator("textures"))
    {
        if (entry.path().extension() == ".png")
            Engine::Dispatcher().trigger<AssetLoadRequest<Texture>>(AssetLoadRequest<Texture>{ entry.path().string() });
    }
}

void TextureSystem::WindDown()
{
    auto& textures = Engine::Res<Texture>();
    for (auto t = textures.begin(); t != textures.end(); t++)
    {
        delete t->second;
    }
    
    textures.clear();

    Engine::Dispatcher().sink<AssetLoadRequest<Texture>>().disconnect<&TextureSystem::OnLoadAsset>(this);
}
