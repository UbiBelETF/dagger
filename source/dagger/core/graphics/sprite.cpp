#include "sprite.h"
#include "shaders.h"
#include "texture.h"
#include "textures.h"
#include "core/engine.h"
#include "imgui.h"

void dagger::AssignSpriteTexture(Sprite& spriteTarget_, String textureName_)
{
	ViewPtr<Texture> texture = TextureSystem::Get(textureName_);

	// we copy this over so that this info can be copied into GPU-space directly
	spriteTarget_.image = texture.get();
	spriteTarget_.size.x = texture->Width();
	spriteTarget_.size.y = texture->Height();
}

void dagger::AssignSpriteTexture(Sprite& spriteTarget_, ViewPtr<Texture> texture_)
{
	spriteTarget_.image = texture_.get();
	spriteTarget_.size.x = texture_->Width();
	spriteTarget_.size.y = texture_->Height();
}

void dagger::AssignSpriteShader(Sprite& spriteTarget_, String shaderName_)
{
	spriteTarget_.shader = ShaderSystem::Get(shaderName_);
}

void dagger::Sprite::PropertiesDrawer(Entity entity_)
{
	if (ImGui::CollapsingHeader("Sprite"))
	{
		auto& sprite = Engine::Registry().get<Sprite>(entity_);

		// scale
		float scale[2] = { sprite.scale.x, sprite.scale.y };
		ImGui::DragFloat2("Scale", scale, 0.01f, 100.f);
		sprite.scale = { scale[0], scale[1] };

		// color
		float color[4] = { sprite.color.r, sprite.color.g, sprite.color.b, sprite.color.a };
		ImGui::ColorPicker4("Tint Color", color);
		sprite.color = { color[0], color[1], color[2], color[3] };
	}
}