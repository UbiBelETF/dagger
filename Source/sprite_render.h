#pragma once

#include "system.h"
#include "core.h"
#include "window.h"
#include "shaders.h"
#include "shader.h"

#include <vector>
#include <cstdint>
#include <memory>

using namespace dagger;

class SpriteRenderSystem
	: public System
	, public Subscriber<Render, ShaderChangeRequest>
{
	const Float32 m_VerticesAndTexCoords[24] = {
		 -0.5f, -0.5f, 0.0f, 0.0f,
		 -0.5f,  0.5f, 0.0f, 1.0f,
		  0.5f,  0.5f, 1.0f, 1.0f,
		  0.5f, -0.5f, 1.0f, 0.0f,
		 -0.5f, -0.5f, 0.0f, 0.0f,
		  0.5f,  0.5f, 1.0f, 1.0f
	};

	UInt32 m_VAO;
	UInt32 m_StaticMeshVBO;
	UInt32 m_InstanceQuadInfoVBO;
	Float32* m_Data;

	UInt8 m_Index = 0;
	Shader* m_CachedShader;

	void OnRender();
	void OnShaderChanged(ShaderChangeRequest request_);

public:
	constexpr static UInt64 ms_VertexCount = 24;
	constexpr static UInt64 m_SizeOfMesh = sizeof(Float32) * ms_VertexCount;
	constexpr static UInt64 ms_MaxNumberOfMeshes = 10000;
	constexpr static UInt64 ms_BufferSize = sizeof(Float32) * ms_VertexCount * ms_MaxNumberOfMeshes;

	void SpinUp() override;
	void WindDown() override;
};