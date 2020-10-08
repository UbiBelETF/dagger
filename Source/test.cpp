#include "test.h"
#include "engine.h"
#include <limits>
#include <algorithm>
#include <execution>

using namespace dagger;

void TestSystem::AddEntities()
{
	auto& reg = Engine::Registry();
	if (reg.size() >= ms_MaxNumberOfMeshes - 10) return;

	for (int i = 0; i < 10; i++)
	{
		auto entity = reg.create();
		reg.emplace<Sprite>(entity, ++Engine::ms_EntityId, ((rand() % 10) - 5) * 0.01f, ((rand() % 10) - 5) * 0.01f);
	}

	spdlog::info("Current entity count: {}", reg.size());
}

void TestSystem::SpinUp(Engine&)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	auto sh = std::make_unique<Shader>("standard", ShaderStage::Vertex | ShaderStage::Fragment);
	m_Shaders.push_back(std::move(sh));

	auto flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;

	for (int i = 0; i < 3; i++)
	{
		glGenBuffers(1, &m_VBOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[i]);
		glBufferStorage(GL_ARRAY_BUFFER, ms_BufferSize, 0, flags);
		glBufferData(GL_ARRAY_BUFFER, ms_BufferSize, nullptr, GL_STREAM_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
		m_Data[i] = reinterpret_cast<float*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, ms_BufferSize, flags | GL_MAP_UNSYNCHRONIZED_BIT));
		assert(m_Data[i] != nullptr);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	glBindVertexArray(0);

	Engine::Dispatch().sink<Render>().connect<&TestSystem::OnRender>(this);
	Engine::Dispatch().sink<KeyboardEvent>().connect<&TestSystem::AddEntities>(this);
}

void TestSystem::OnRender()
{
	glBindVertexArray(m_VAO);

	auto result = glClientWaitSync(m_Fences[m_Index], 0, 1000 * 1000 * 100);
	if (result == GL_TIMEOUT_EXPIRED)
	{
		Engine::Dispatch().trigger<Error>(Error("glClientWaitSync failed."));
		return;
	}

	glDeleteSync(m_Fences[m_Index]);

	assert(m_VBOs[m_Index] != 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[m_Index]);

	auto entities = Engine::Registry().view<Sprite>();

	auto* temp = m_Data[m_Index];
	std::for_each(std::execution::par_unseq, entities.begin(), entities.end(), [=](auto entity)
	{
		Sprite& sprite = entities.get<Sprite>(entity);
		const std::size_t index = sprite.m_Id * 6;

		float& x = sprite.m_XY[0];
		float& y = sprite.m_XY[1];

		x += (rand() % 3 - 1) * 0.001f;
		y += (rand() % 3 - 1) * 0.001f;

		m_Data[m_Index][index + 0] = x - 0.5f;
		m_Data[m_Index][index + 1] = y - 0.5f;

		m_Data[m_Index][index + 2] = x + 0.5f;
		m_Data[m_Index][index + 3] = y - 0.5f;

		m_Data[m_Index][index + 4] = x + 0.0f;
		m_Data[m_Index][index + 5] = y + 0.5f;
	});
	
	glUseProgram(m_Shaders[0]->m_ProgramId);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, ms_VertexCount * ms_MaxNumberOfMeshes);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glUseProgram(0);

	m_Fences[m_Index] = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
	m_Index = (m_Index + 1) % 3;
}

void TestSystem::WindDown(Engine&)
{
	for (auto& shader : m_Shaders)
		shader.reset();

	for (int i = 0; i < 3; i++)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[i]);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDeleteBuffers(1, &m_VBOs[i]);
	}

	glDeleteVertexArrays(1, &m_VAO);

	Engine::Dispatch().sink<Render>().disconnect<&TestSystem::OnRender>(this);
}
