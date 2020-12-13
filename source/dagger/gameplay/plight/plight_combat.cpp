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

#include "gameplay/plight/plight_controller.h"



#define BAR_START_SIZE 50.f

using namespace plight;

void PlightCombatSystem::Run()
{

	auto view = Engine::Registry().view <PlightCollision, Transform, PlightCharacterController, CombatStats >();
	for (auto entity : view) {
		PlightCollision& col = Engine::Registry().get<PlightCollision>(entity);
		Transform& t = Engine::Registry().get<Transform>(entity);
		PlightCharacterController& character = Engine::Registry().get<PlightCharacterController>(entity);

		CombatStats& cstats = Engine::Registry().get<CombatStats>(entity);

		cstats.currentTimer += Engine::DeltaTime();
		if (cstats.currentTimer >= cstats.updateTimer) {
			//Will add conditions based on the current character state (to be implemented in character controller system)
			if (col.colided) {
				auto it = col.colidedWith.begin();
				while (it != col.colidedWith.end()) {
					if (Engine::Registry().valid(*it)) {
						if (Engine::Registry().has<PlightCharacterController>(*it)) {
							auto& ch = Engine::Registry().get<CombatStats>(*it);
							auto& pchar = Engine::Registry().get<PlightCharacterController>(*it);


							ch.currentHealth -= 0.1f;

							if (ch.currentHealth <= 0.f) {
								ch.currentHealth = 0.f;
							}

							auto& sprite = Engine::Registry().get<Sprite>(ch.currentHealthBar);
							ch.healthBarOffset += (sprite.size.x - (BAR_START_SIZE * (ch.currentHealth / ch.maxHealth))) / 2;
							sprite.size.x = BAR_START_SIZE * (ch.currentHealth / ch.maxHealth);


						}

					}
					it++;
				}
			}

			//Stamina will not be affecting running , it will be used for special movement like dashing or rolling when it gets implemented (used on running for example here)

			if (character.running) {

				cstats.currentStamina -= STAMINA_FOR_RUNNING_FRAME;
				if (cstats.currentStamina < STAMINA_FOR_RUNNING_FRAME) {
					if (cstats.currentStamina < 0.f) {
						cstats.currentStamina = 0.f;
					}
					character.running = false;
					character.resting = true;

				}
				auto& sprite = Engine::Registry().get<Sprite>(cstats.currentStaminaBar);
				cstats.staminaBarOffset -= (sprite.size.x - (BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina))) / 2;
				sprite.size.x = BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina);
			}
			else {

				cstats.currentStamina += STAMINA_FOR_RUNNING_FRAME / 2;
				if (cstats.currentStamina > 100) {
					cstats.currentStamina = 100;
				}
				if (cstats.currentStamina < 100) {
					auto& sprite = Engine::Registry().get<Sprite>(cstats.currentStaminaBar);
					cstats.staminaBarOffset += std::abs((sprite.size.x - (BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina))) / 2);
					sprite.size.x = BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina);
				}
			}
			cstats.currentTimer = 0.f;
		}

		auto& characterSprite = Engine::Registry().get<Transform>(entity);
		auto& characterBackgroundHealthSprite = Engine::Registry().get<Sprite>(cstats.backgroundHealthBar);
		auto& characterCurrentHealthSprite = Engine::Registry().get<Sprite>(cstats.currentHealthBar);
		auto& characterBackgroundStaminaSprite = Engine::Registry().get<Sprite>(cstats.backgroundStaminaBar);
		auto& characterCurrentStaminaSprite = Engine::Registry().get<Sprite>(cstats.currentStaminaBar);

		characterBackgroundHealthSprite.position.y = characterSprite.position.y + cstats.playerDistance + 10.f;
		characterBackgroundHealthSprite.position.x = characterSprite.position.x;

		characterCurrentHealthSprite.position.y = characterSprite.position.y + cstats.playerDistance + 10.f;
		characterCurrentHealthSprite.position.x = characterSprite.position.x;
		characterCurrentHealthSprite.position.x += cstats.healthBarOffset;

		characterBackgroundStaminaSprite.position.y = characterSprite.position.y + cstats.playerDistance;
		characterBackgroundStaminaSprite.position.x = characterSprite.position.x;

		characterCurrentStaminaSprite.position.y = characterSprite.position.y + cstats.playerDistance;
		characterCurrentStaminaSprite.position.x = characterSprite.position.x;
		characterCurrentStaminaSprite.position.x += cstats.staminaBarOffset;
	}

}