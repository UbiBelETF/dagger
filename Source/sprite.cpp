#include "sprite.h"
#include "shaders.h"
#include "texture.h"
#include "textures.h"

void Sprite::UseTexture(String textureName_)
{
	Texture* texture = TextureSystem::Get(textureName_);

	// we copy this over so that this info can be copied into GPU-space directly
	m_Image = texture->Index();
	m_Ratio = texture->Ratio();
}

void Sprite::UseShader(String shaderName_)
{
	m_Shader = ShaderSystem::Get(shaderName_);
}
