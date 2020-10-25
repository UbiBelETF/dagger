#pragma once

#include "window.h"
#include "shaders.h"
#include "shader.h"
#include "core/system.h"
#include "core/core.h"

#include <vector>
#include <cstdint>
#include <memory>

using namespace dagger;

class SpriteRenderSystem
	: public System
	, public Subscriber<Render, ShaderChangeRequest>
{
	const Float32 m_VerticesAndTexCoords[24] = {
		 -0.55f, -0.55f, 0.0f, 0.0f,
		 -0.55f,  0.55f, 0.0f, 1.0f,
		  0.55f,  0.55f, 1.0f, 1.0f,
		  0.55f, -0.55f, 1.0f, 0.0f,
		 -0.55f, -0.55f, 0.0f, 0.0f,
		  0.55f,  0.55f, 1.0f, 1.0f
	};

	UInt32 m_VAO;
	UInt32 m_StaticMeshVBO;
	UInt32 m_InstanceQuadInfoVBO;
	Float32* m_Data;

	UInt8 m_Index = 0;
	ViewPtr<Shader> m_CachedShader;

	void OnRender();
	void OnShaderChanged(ShaderChangeRequest request_);

public:
	inline String SystemName() { return "Sprite Render System"; }

	constexpr static UInt64 ms_VertexCount = 24;
	constexpr static UInt64 m_SizeOfMesh = sizeof(Float32) * ms_VertexCount;
	constexpr static UInt64 ms_MaxNumberOfMeshes = 10000;
	constexpr static UInt64 ms_BufferSize = sizeof(Float32) * ms_VertexCount * ms_MaxNumberOfMeshes;

	void SpinUp() override;
	void WindDown() override;
};