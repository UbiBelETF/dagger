#include "sprite.h"
#include "shaders.h"
#include "texture.h"
#include "textures.h"
#include "core/engine.h"

using namespace dagger;

void SpriteCutoutData::Use(SpriteFrame* spritesheet_)
{
	subSize = spritesheet_->frame.subSize;
	subOrigin = spritesheet_->frame.subOrigin;
	size = spritesheet_->frame.size;
}

void dagger::AssignSprite(Sprite& spriteTarget_, String textureName_, UInt32 size_)
{
	if (Engine::Res<SpriteFrame>().contains(textureName_))
	{
		AssignSprite(spriteTarget_, Engine::Res<SpriteFrame>()[textureName_]);
		return;
	}
	ViewPtr<Texture> texture = TextureSystem::Get(textureName_);

	spriteTarget_.image = texture.get();
	spriteTarget_.size.x = texture->Width();
	spriteTarget_.size.y = texture->Height();
	spriteTarget_.size *= size_;
	spriteTarget_.UseFullImage();
}

void dagger::AssignSprite(Sprite& spriteTarget_, ViewPtr<Texture> texture_)
{
	spriteTarget_.image = texture_.get();
	spriteTarget_.size.x = texture_->Width();
	spriteTarget_.size.y = texture_->Height();
	spriteTarget_.UseFullImage();
}

void dagger::AssignSprite(Sprite& spriteTarget_, ViewPtr<SpriteFrame> spritesheet_)
{
	spriteTarget_.image = spritesheet_->texture.get();
	spriteTarget_.size.x = spritesheet_->texture->Width();
	spriteTarget_.size.y = spritesheet_->texture->Height();
	spriteTarget_.Use(spritesheet_.get());
}

void dagger::AssignSpriteShader(Sprite& spriteTarget_, String shaderName_)
{
	spriteTarget_.shader = ShaderSystem::Get(shaderName_);
}
