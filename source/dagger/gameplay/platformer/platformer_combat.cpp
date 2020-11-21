#include "platformer_combat.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

#include "gameplay/platformer/platformer_collision.h"
#include "gameplay/platformer/platformer_controller.h"
#include "gameplay/platformer/platformer_main.h"

#include "gameplay/platformer/character_complete_definition.h"


using namespace platformer;

void PlatformerCombatSystem::Run()
{
	auto view = Engine::Registry().view <PlatformerCollision, Transform, PlatformerCharacter, CharacterHealth > ();
	for (auto entity : view) {
		PlatformerCollision& col = Engine::Registry().get<PlatformerCollision>(entity);
		Transform& t = Engine::Registry().get<Transform>(entity);
		PlatformerCharacter& character = Engine::Registry().get<PlatformerCharacter>(entity);
		CharacterHealth& chealth = Engine::Registry().get<CharacterHealth>(entity);

		if (character.dead) {
			return;
		}
		if (col.colided && (character.heavy || character.down || character.light)) {
			if (Engine::Registry().valid(col.colidedWith)) {
				auto& ch = Engine::Registry().get<CharacterHealth>(col.colidedWith);
				auto& pchar = Engine::Registry().get<PlatformerCharacter>(col.colidedWith);

				if (pchar.dead || pchar.died) {
					continue;
				}

				if (character.heavy) {
					ch.currentHealth -= 0.05f;
					
				}
				else if (character.down) {
					ch.currentHealth -= 0.025f;
				
				}
				else if (character.light) {
					ch.currentHealth -= 0.01f;
				
				}
				if (ch.currentHealth <= 0.f) {
					ch.currentHealth = 0.f;
					pchar.died = true;
				}

				
					auto c = Character::Get(col.colidedWith);
					auto viewSprites = Engine::Registry().view<Sprite>();
					for (auto e : viewSprites) {
						if (e == c.chealth.currentHealthBar) {
							auto& sprite = Engine::Registry().get<Sprite>(e);
							sprite.size.x = HEALTH_BAR_START_SIZE * (ch.currentHealth / ch.maxHealth);
						}
					}
				

			}
		}
	}

}
