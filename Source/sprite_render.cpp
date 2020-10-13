#include "sprite_render.h"
#include "engine.h"
#include "textures.h"
#include "texture.h"

#include <limits>
#include <algorithm>
#include <execution>
#include <chrono>

using namespace dagger;

void SpriteRenderSystem::AddEntities()
{
	auto& reg = Engine::Registry();
	if (reg.size() >= ms_MaxNumberOfMeshes - 10) return;

	for (int i = 0; i < 10; i++)
	{
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        sprite.m_Color.R = 1.0f;
        sprite.m_Color.G = 1.0f;
        sprite.m_Color.B = 1.0f;
        sprite.m_Color.A = 1.0f;
        sprite.m_Position.Z = 0.001f * Engine::ms_EntityId++;
        sprite.m_Image = TextureSystem::Get(rand() % 2 ? "rayman" : "dagger");
    }

	spdlog::info("Current entity count: {}", reg.size());
}

void SpriteRenderSystem::SpinUp()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

    Engine::Res<Shader>()["standard"] = m_CurrentShader = new Shader("standard", ShaderStage::Vertex | ShaderStage::Fragment);    

    auto flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	glGenBuffers(1, &m_StaticMeshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_StaticMeshVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), m_Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    GLsizei size = sizeof(float) * 9 + sizeof(unsigned int);
    assert(size == sizeof(Sprite));

    glGenBuffers(1, &m_InstanceQuadInfoVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_InstanceQuadInfoVBO);
    glBufferStorage(GL_ARRAY_BUFFER, ms_BufferSize, 0, flags);
	glBufferData(GL_ARRAY_BUFFER, ms_BufferSize, nullptr, GL_STREAM_DRAW);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, size, (void*)0); // position
    glEnableVertexAttribArray(2);
    glVertexAttribDivisor(2, 1);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, size, (void*)(sizeof(float) * 3)); // pivot
    glEnableVertexAttribArray(3);
    glVertexAttribDivisor(3, 1);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, size, (void*)(sizeof(float) * 5)); // color
    glEnableVertexAttribArray(4);
    glVertexAttribDivisor(4, 1);
    glVertexAttribPointer(5, 1, GL_UNSIGNED_INT, GL_FALSE, size, (void*)(sizeof(float) * 9)); // image handle
    glEnableVertexAttribArray(5);
    glVertexAttribDivisor(5, 1);

	m_Data = reinterpret_cast<float*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, ms_BufferSize, flags));
	assert(m_Data != nullptr);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);

    glUseProgram(m_CurrentShader->m_ProgramId);
    Engine::Dispatcher().trigger<TextureUploadRequest>(TextureUploadRequest{ m_CurrentShader->m_ProgramId, 0 });
    glUseProgram(0);

    Engine::Dispatcher().sink<Render>().connect<&SpriteRenderSystem::OnRender>(this);
	Engine::Dispatcher().sink<KeyboardEvent>().connect<&SpriteRenderSystem::AddEntities>(this);
}

void SpriteRenderSystem::OnRender()
{
	glBindVertexArray(m_VAO);
    glUseProgram(m_CurrentShader->m_ProgramId);

	assert(m_StaticMeshVBO != 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_StaticMeshVBO);
    
    auto entities = Engine::Registry().view<Sprite>();
    auto size = sizeof(Sprite) * entities.size();
    
    memcpy_s(m_Data, size, entities.raw(), size);

    glDrawArraysInstanced(GL_TRIANGLES, 0, ms_VertexCount, (GLsizei)entities.size());

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void SpriteRenderSystem::WindDown()
{
    delete m_CurrentShader;

    glBindBuffer(GL_ARRAY_BUFFER, m_StaticMeshVBO);
    glUnmapBuffer(GL_ARRAY_BUFFER);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteBuffers(1, &m_StaticMeshVBO);

	glDeleteVertexArrays(1, &m_VAO);

	Engine::Dispatcher().sink<Render>().disconnect<&SpriteRenderSystem::OnRender>(this);
}
