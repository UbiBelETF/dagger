#include "camera.h"

#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/graphics/window.h"

#include "gameplay/team_game/character_controller.h"

using namespace dagger;
using namespace team_game;

void CameraSystem::Run()
{
	auto* camera = Engine::GetDefaultResource<Camera>();

	auto view = Engine::Registry().view<CharacterController, Sprite>();

	for (auto entity : view)
	{
		auto sprite = view.get<Sprite>(entity);

		camera->position = { sprite.position.x, sprite.position.y, camera->position.z };
	}
}
