#include "text.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"

using namespace dagger;

void Text::Set(String font, String message_, Vector3 pos_)
{
	this->font = font;

	auto& registry = Engine::Registry();

	assert(Engine::Res<Texture>().contains(fmt::format("spritesheets:{}", font)));

	if (entities.size() > 0)
		registry.remove(entities.begin(), entities.end());

	int positionX = position.x;
	auto& sheets = Engine::Res<Spritesheet>();
	for (char letter : message_)
	{
		auto& spritesheet = sheets[fmt::format("spritesheets:{}:{}", font, (int)letter)];
		auto entity = registry.create();
		auto& sprite = registry.emplace<Sprite>(entity);
		sprite.position = { positionX, position.y, position.z };
		AssignSprite(sprite, spritesheet);

		positionX += (int)(spritesheet->frame.size.x * spacing);
	}
}