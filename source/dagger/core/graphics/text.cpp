#include "text.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"

using namespace dagger;

void Text::Set(String font_, String message_, Vector3 pos_, Bool ui_)
{
	font = font_;
	position=pos_;

	auto& registry = Engine::Registry();

	assert(Engine::Res<Texture>().contains(fmt::format("spritesheets:{}", font_)));

	if (entities.size() > 0)
	{
		registry.remove(entities.begin(), entities.end());
		registry.destroy(entities.begin(), entities.end());
		entities.clear();
	}

	SInt32 positionX = position.x;
	auto& sheets = Engine::Res<SpriteFrame>();

	Map<UInt32, SpriteFrame*> cache;
	UInt32 fullStringWidth = 0;
	for (char letter : message_)
	{
		cache[letter] = sheets[fmt::format("spritesheets:{}:{}", font, (int)letter)];
		fullStringWidth += cache[letter]->frame.size.x * spacing;
	}

	Float32 xOffsetDueToAlign = 0.0f;
	if (alignment == TextAlignment::CENTER)
		xOffsetDueToAlign = (Float32)fullStringWidth / 2.0f;
	else if (alignment == TextAlignment::RIGHT)
		xOffsetDueToAlign = (Float32)fullStringWidth;

	for (char letter : message_)
	{
		auto spritesheet = cache[letter];
		auto entity = registry.create();
		auto& sprite = registry.emplace<Sprite>(entity);

		if (ui_) sprite.UseAsUI();
		sprite.position = { positionX - xOffsetDueToAlign, position.y, position.z };
		AssignSprite(sprite, spritesheet);

		positionX += (int)(spritesheet->frame.size.x * spacing);
		entities.push_back(entity);
	}
}