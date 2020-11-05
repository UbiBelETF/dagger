#include "sprite.h"
#include "shaders.h"
#include "texture.h"
#include "textures.h"

void dagger::AssignSpriteTexture(Sprite& spriteTarget_, String textureName_)
{
	ViewPtr<Texture> texture = TextureSystem::Get(textureName_);

	// we copy this over so that this info can be copied into GPU-space directly
	spriteTarget_.image = texture.get();
	spriteTarget_.ratio = texture->Ratio();
}

void dagger::AssignSpriteTexture(Sprite& spriteTarget_, ViewPtr<Texture> texture_)
{
	spriteTarget_.image = texture_.get();
	spriteTarget_.ratio = texture_->Ratio();
}

void dagger::AssignSpriteShader(Sprite& spriteTarget_, String shaderName_)
{
	spriteTarget_.shader = ShaderSystem::Get(shaderName_);
}
