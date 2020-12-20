#include "traps_collision.h"

#include "core/engine.h"
#include "core/graphics/window.h"
#include "core/graphics/text.h"

#include "gameplay/team_game/team_game_collisions.h"
#include "gameplay/team_game/character_controller.h"

using namespace team_game;

void TrapsCollisionSystem::SpinUp()
{
	Engine::Dispatcher().sink<NextFrame>().connect<&TrapsCollisionSystem::OnEndOfFrame>(this);
}

void TrapsCollisionSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&TrapsCollisionSystem::OnEndOfFrame>(this);
}

void TrapsCollisionSystem::Run()
{
	auto& playerView = Engine::Registry().view<PlayerCharacter, Collider>();
	
	for (auto& entity : playerView)
	{
		auto& collider = Engine::Registry().get<Collider>(entity);
		if (collider.listOfEntities.size() > 0)
		{
			for (auto& other : collider.listOfEntities)
			{
				auto& otherCollision = Engine::Registry().get<Collider>(other);
				if (otherCollision.entityType == CollisionID::TRAP)
				{
					auto& character = Engine::Registry().get<PlayerCharacter>(entity);
					collided[character.id] = true;
				}
			}
		}
	}
}


void TrapsCollisionSystem::OnEndOfFrame()
{
	auto& playerView = Engine::Registry().view<PlayerCharacter>();
	for (auto& entity : playerView)
	{
		auto& character = Engine::Registry().get<PlayerCharacter>(entity);
		if (collided[character.id])
		{
			Logger::trace(fmt::format("Game over. Player {} died.", character.id));
			Engine::Registry().destroy(entity);

			auto gameOverEntity = Engine::Registry().create();
			auto& gameOverText = Engine::Registry().emplace<Text>(gameOverEntity);
			gameOverText.alignment = { TextAlignment::CENTER };
			gameOverText.spacing = 0.5f;

			auto* camera = Engine::GetDefaultResource<Camera>();

			gameOverText.Set("pixel-font", "GAME OVER", camera->position);
		}
	}
}

