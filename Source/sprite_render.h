#pragma once

#include "system.h"
#include "structures.h"
#include "window.h"
#include "shader.h"

#include <vector>
#include <cstdint>
#include <memory>

using namespace dagger;

class SpriteRenderSystem
	: public System
	, public Subscriber<Render>
{
	const float m_Vertices[24] = {
		 -0.5f, -0.5f, 0.0f, 0.0f,
		 -0.5f,  0.5f, 0.0f, 1.0f,
		  0.5f,  0.5f, 1.0f, 1.0f,
		  0.5f, -0.5f, 1.0f, 0.0f,
		 -0.5f, -0.5f, 0.0f, 0.0f,
		  0.5f,  0.5f, 1.0f, 1.0f
	};

	unsigned int m_VAO;

	GLuint m_StaticMeshVBO;
	GLuint m_InstanceQuadInfoVBO;
	float* m_Data;

	unsigned char m_Index = 0;

	Shader* m_CurrentShader;

	void OnRender();
	void AddEntities();

public:
	constexpr static uint64_t ms_VertexCount = 24;
	constexpr static uint64_t m_SizeOfMesh = sizeof(float) * ms_VertexCount;
	constexpr static uint64_t ms_MaxNumberOfMeshes = 10000;
	constexpr static uint64_t ms_BufferSize = sizeof(float) * ms_VertexCount * ms_MaxNumberOfMeshes;


	void SpinUp() override;
	void WindDown() override;
};