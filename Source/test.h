#pragma once

#include "system.h"
#include "structures.h"
#include "window.h"
#include "shader.h"

#include <vector>
#include <cstdint>
#include <memory>

using namespace dagger;

class TestSystem
	: public System
	, public Subscriber<Render>
{
	const float m_Vertices[6] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.0f,  0.5f
	};

	unsigned int m_VAO;

	unsigned char m_Index = 0;
	GLsync m_Fences[3];
	GLuint m_VBOs[3];
	float* m_Data[3];

	std::vector<std::unique_ptr<Shader>> m_Shaders;

	void OnRender();
	void AddEntities();

public:
	constexpr static uint64_t ms_VertexCount = 6;
	constexpr static uint64_t m_SizeOfMesh = sizeof(float) * ms_VertexCount;
	constexpr static uint64_t ms_MaxNumberOfMeshes = 10000;
	constexpr static uint64_t ms_BufferSize = sizeof(float) * ms_VertexCount * ms_MaxNumberOfMeshes;


	void SpinUp(Engine&) override;
	void WindDown(Engine&) override;
};