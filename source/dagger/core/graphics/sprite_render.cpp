#include "sprite_render.h"
#include "sprite.h"
#include "textures.h"
#include "texture.h"
#include "core/engine.h"

#include <limits>
#include <algorithm>
#include <execution>
#include <chrono>
#include <cstdint>
#include <regex>

using namespace dagger;

void SpriteRenderSystem::SpinUp()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_StaticMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_StaticMeshVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_VerticesAndTexCoords), m_VerticesAndTexCoords, GL_STATIC_DRAW);

    // attribute #0: vertex position
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 
        sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    // attribute #1: tex coord
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 
        sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    const GLsizei dataSize = sizeof(SpriteData);
    
    glGenBuffers(1, &m_InstanceQuadInfoVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceQuadInfoVBO);
	glBufferData(GL_ARRAY_BUFFER, s_BufferSize, nullptr, GL_STREAM_DRAW);

    const StaticArray<Pair<UInt32, UInt32>, 7> sizesAndStrides = {
        pair(2, 0),     // #2: sub size
        pair(2, 2),     // #3: sub origin
        pair(2, 4),     // #4: sub origin
        pair(3, 6),     // #5: quad position
        pair(2, 9),     // #6: quad pivot
        pair(4, 11),    // #7: quad tint color
        pair(2, 15),    // #8: scale
    };

    for (UInt32 i = 0; i < sizesAndStrides.size(); i++)
    {
        const UInt32 index = 2 + i;
        glVertexAttribPointer(index, sizesAndStrides[i].first, GL_FLOAT, GL_FALSE, 
            dataSize, (void*)(sizeof(Float32) * sizesAndStrides[i].second));
        glEnableVertexAttribArray(index);
        glVertexAttribDivisor(index, 1);
    }

	glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

    Engine::Dispatcher().sink<AssetLoadRequest<SpriteFrame>>().connect<&SpriteRenderSystem::OnRequestSpritesheet>(this);

    for (auto& entry : Files::recursive_directory_iterator("spritesheets"))
    {
        if (entry.path().extension() == ".spritesheet")
            Engine::Dispatcher().trigger<AssetLoadRequest<SpriteFrame>>(AssetLoadRequest<SpriteFrame>{ entry.path().string() });
    }

    Engine::Dispatcher().sink<Render>().connect<&SpriteRenderSystem::OnRender>(this);
}

void SpriteRenderSystem::OnRequestSpritesheet(AssetLoadRequest<SpriteFrame> request_)
{
    FilePath path{ request_.path };
    String name = path.stem().string();
    auto& textures = Engine::Res<Texture>();
    if (!textures.contains(name))
        Engine::Dispatcher().trigger<AssetLoadRequest<Texture>>(
            AssetLoadRequest<Texture>{ path.replace_extension("png").string() });

    FilePath root{ request_.path };
    root.remove_filename();

    String textureName = "";
    {
        String pathName = root.append(path.stem().string()).string();
        if (pathName.find("textures") == 0)
            pathName = pathName.substr(9, pathName.length() - 13);

        std::replace(pathName.begin(), pathName.end(), '/', ':');
        std::replace(pathName.begin(), pathName.end(), '\\', ':');
        textureName = pathName;
    }

    assert(textures.contains(textureName));
    auto texture = textures[textureName];

    std::ifstream framesInput{ request_.path };
    String line;

    std::regex emptyLine("^[ \t\n]*$");
    std::regex animationLine("([a-zA-Z_0-9]+)[ \t\n]+([0-9]+)[ \t\n]+([0-9]+)[ \t\n]+([0-9]+)[ \t\n]+([0-9]+)[ \t\n]+([0-9]+)");
    std::regex spriteLine("([a-zA-Z_0-9]+)[ \t\n]+([0-9]+)[ \t\n]+([0-9]+)[ \t\n]+([0-9]+)[ \t\n]+([0-9]+)");

    while (std::getline(framesInput, line))
    {
        if (std::regex_match(line, emptyLine))
            continue;

        std::smatch match;

        UInt32 count = 1;
        if (std::regex_match(line, match, animationLine))
        {
            count = std::atoi(match[6].str().c_str());
        }
        else
        {
            assert(std::regex_match(line, match, spriteLine));
        }

        Vector2 fullSize{ texture->Width(), texture->Height() };

        auto spriteName = match[1].str();
        auto x = std::atoi(match[2].str().c_str());
        auto y = std::atoi(match[3].str().c_str());
        auto w = std::atoi(match[4].str().c_str());
        auto h = std::atoi(match[5].str().c_str());

        for (UInt32 i = 0; i < count; ++i)
        {
            SpriteFrame* spritesheet = new SpriteFrame();
            spritesheet->texture = texture;

            spritesheet->frame.size.x = w;
            spritesheet->frame.size.y = h;

            spritesheet->frame.subSize.x = (Float32)w / fullSize.x;
            spritesheet->frame.subSize.y = (Float32)h / fullSize.y;

            spritesheet->frame.subOrigin.x = (Float32)(x + w * i) / fullSize.x;
            spritesheet->frame.subOrigin.y = 1.0f - spritesheet->frame.subSize.y - (Float32)y / fullSize.y;

            auto fullSpriteName = fmt::format(count > 1 ? "{}:{}:{}" : "{}:{}", 
                textureName, spriteName, i + 1);

            Engine::Res<SpriteFrame>()[fullSpriteName] = spritesheet;

            Logger::info("Spritesheet loaded: {} -> {} {} {} {} {}",
                textureName.c_str(), fullSpriteName, x, y, w, h);
        }
    }
}

void SpriteRenderSystem::OnRender()
{
	glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceQuadInfoVBO);

    // get a view of all the entities and their sprite components
    ViewPtr<Texture> prevTexture{ nullptr };
    ViewPtr<Shader> prevShader{ nullptr };

    const auto& view = Engine::Registry().view<Sprite>();
    Sequence<Sprite> sprites{ view.raw(), view.raw() + view.size() };
    UInt64 dataSize = sizeof(SpriteData) * sprites.size();
    Sequence<SpriteData> currentRender{};

    std::sort(sprites.begin(), sprites.end(), [](const Sprite& a_, const Sprite& b_)
        {
            UInt32 aShader = a_.shader->programId;
            UInt32 bShader = b_.shader->programId;
            UInt32 aZ = a_.position.z;
            UInt32 bZ = b_.position.z;
            UInt32 aImage = a_.image == nullptr ? 0 : a_.image->TextureId();
            UInt32 bImage = b_.image == nullptr ? 0 : b_.image->TextureId();

            if (aZ == bZ)
            {
                if (aShader == bShader)
                {
                    return aImage < bImage;
                }
                else
                    return aShader < bShader;
            }
            else
                return aZ > bZ;
        });

    for (auto ptr = sprites.begin(); ptr != sprites.end();)
    {
        if (prevShader != ptr->shader)
        {
            prevShader = ptr->shader;
            glUseProgram(prevShader->programId);
            Engine::Dispatcher().trigger<ShaderChangeRequest>(ShaderChangeRequest(prevShader));
        }

        assert(ptr->image != nullptr);
        while (ptr != sprites.end() && ptr->image == nullptr) ptr++;
        if (ptr == sprites.end()) break;

        prevTexture = ptr->image;

        while (ptr != sprites.end() && prevTexture == ptr->image)
        {
            // look at the definition of SpriteData if you're wondering why the cast.
            // we only need some fields, to optimize on data transfer.
            currentRender.push_back((SpriteData)*ptr);
            ptr++;
        }

        const UInt32 renderSize = sizeof(SpriteData) * currentRender.size();

        m_Data = reinterpret_cast<float*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, sizeof(Sprite) * currentRender.size(), GL_MAP_WRITE_BIT));
        memcpy(m_Data, &(*currentRender.begin()), renderSize);
        glUnmapBuffer(GL_ARRAY_BUFFER);

        glBindTexture(GL_TEXTURE_2D, prevTexture->TextureId());
        glDrawArraysInstanced(GL_TRIANGLES, 0, s_VertexCount, (GLsizei)currentRender.size());
        currentRender.clear();
                   
        if (ptr == sprites.end()) break;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderSystem::WindDown()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_StaticMeshVBO);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_StaticMeshVBO);

	glDeleteVertexArrays(1, &m_VAO);

	Engine::Dispatcher().sink<Render>().disconnect<&SpriteRenderSystem::OnRender>(this);
}
