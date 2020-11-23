#include "plight_combat.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

#include "gameplay/plight/plight_collisions.h"
#include "gameplay/plight/topdown_controller.h"


#define HEALTH_BAR_START_SIZE 50.f

using namespace plight;

void PlightCombatSystem::Run()
{
	auto view = Engine::Registry().view <PlightCollision, Transform, TopdownCharacter, CombatStats >();
	for (auto entity : view) {
		PlightCollision& col = Engine::Registry().get<PlightCollision>(entity);
		Transform& t = Engine::Registry().get<Transform>(entity);
		TopdownCharacter& character = Engine::Registry().get<TopdownCharacter>(entity);
		CombatStats& cstats = Engine::Registry().get<CombatStats>(entity);

		//Will add conditions based on the current character state (to be implemented in character controller system)
		if (col.colided) {
			auto it = col.colidedWith.begin();
			while (it != col.colidedWith.end()) {
				if (Engine::Registry().valid(*it)) {
					if (Engine::Registry().has<TopdownCharacter>(*it)) {
						auto& ch = Engine::Registry().get<CombatStats>(*it);
						auto& pchar = Engine::Registry().get<TopdownCharacter>(*it);

						ch.currentHealth -= 0.1f;

						if (ch.currentHealth <= 0.f) {
							ch.currentHealth = 0.f;
						}

						auto& sprite = Engine::Registry().get<Sprite>(ch.currentHealthBar);	
						sprite.position.x -= (sprite.size.x - (  HEALTH_BAR_START_SIZE * (ch.currentHealth / ch.maxHealth)))/2;
						sprite.size.x = HEALTH_BAR_START_SIZE * (ch.currentHealth / ch.maxHealth);
						
						
					}

				}
				it++;
			}
		}
	}

}