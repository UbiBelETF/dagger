#include "sprite_render.h"
#include "sprite.h"
#include "textures.h"
#include "texture.h"
#include "core/engine.h"

#include <limits>
#include <algorithm>
#include <execution>
#include <chrono>

using namespace dagger;

void SpriteRenderSystem::SpinUp()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

    auto flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glGenBuffers(1, &m_StaticMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_StaticMeshVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_VerticesAndTexCoords), m_VerticesAndTexCoords, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    GLsizei size = sizeof(Sprite);
    
    glGenBuffers(1, &m_InstanceQuadInfoVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceQuadInfoVBO);
    glBufferStorage(GL_ARRAY_BUFFER, ms_BufferSize, 0, flags);
	glBufferData(GL_ARRAY_BUFFER, ms_BufferSize, nullptr, GL_STREAM_DRAW);

    const UInt32 attributeSizes[]   = { 3,        2,        4,        1,               1,        1 };
    const UInt64 attributeStrides[] = { 0,        3,        5,        9,               10,       11 };

    for (UInt32 i = 0; i < 6; i++)
    {
        UInt32 index = 2 + i;
        glVertexAttribPointer(index, attributeSizes[i], GL_FLOAT, GL_FALSE, size, (void*)(sizeof(Float32) * attributeStrides[i]));
        glEnableVertexAttribArray(index);
        glVertexAttribDivisor(index, 1);
    }

	m_Data = reinterpret_cast<float*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, ms_BufferSize, flags));
	assert(m_Data != nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

    Engine::Dispatcher().sink<Render>().connect<&SpriteRenderSystem::OnRender>(this);
    Engine::Dispatcher().sink<ShaderChangeRequest>().connect<&SpriteRenderSystem::OnShaderChanged>(this);
}

void SpriteRenderSystem::OnShaderChanged(ShaderChangeRequest request_)
{
    m_CachedShader = request_.m_Shader;
}

void SpriteRenderSystem::OnRender()
{
    assert(m_CachedShader != nullptr);

	glBindVertexArray(m_VAO);
    
	assert(m_StaticMeshVBO != 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_StaticMeshVBO);

    // get a view of all the entities and their sprite components
    auto entities = Engine::Registry().view<Sprite>();
    std::size_t size = sizeof(Sprite) * entities.size();
    
    memcpy_s(m_Data, size, entities.raw(), size);

    glDrawArraysInstanced(GL_TRIANGLES, 0, ms_VertexCount, (GLsizei)entities.size());

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
    Engine::Dispatcher().sink<ShaderChangeRequest>().disconnect<&SpriteRenderSystem::OnShaderChanged>(this);
}
