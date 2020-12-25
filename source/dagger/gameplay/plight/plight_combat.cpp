#include "plight_combat.h"
#include "plight_game_logic.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"

#include "gameplay/plight/plight_collisions.h"
#include "gameplay/plight/plight_particles.h"
#include "gameplay/plight/plight_projectiles.h"
#include "gameplay/plight/plight_controller.h"
#include "gameplay/plight/plight_melee.h"





using namespace plight;

void PlightCombatSystem::Run()
{
	auto viewLS1 = Engine::Registry().view<PlightIntro>();
	auto it = viewLS1.begin();
	auto& pin = viewLS1.get<PlightIntro>(*it);

	if (pin.IsFinished()) {

		auto view = Engine::Registry().view <PlightCollision, Transform, PlightCharacterController, CombatStats >();
		for (auto entity : view) {
			PlightCollision& col = Engine::Registry().get<PlightCollision>(entity);
			Transform& t = Engine::Registry().get<Transform>(entity);
			PlightCharacterController& character = Engine::Registry().get<PlightCharacterController>(entity);

			CombatStats& cstats = Engine::Registry().get<CombatStats>(entity);

		if (character.dead) {
			continue;
		}


		cstats.currentTimer += Engine::DeltaTime();
		
			//Will add conditions based on the current character state (to be implemented in character controller system)
			if (col.colided) {
				auto it = col.colidedWith.begin();
				while (it != col.colidedWith.end()) {
					if (Engine::Registry().valid(*it)) {
						if (Engine::Registry().has<Weapon>(*it)) {
							if (cstats.currentTimer >= cstats.updateTimer) {
								
								auto& weapon = Engine::Registry().get<Weapon>(*it);
							if (weapon.attacking) {
								cstats.currentHealth -= weapon.weaponDamage;
							}
								

								if (cstats.currentHealth <= 0.f) {
									cstats.currentHealth = 0.f;
								}
								
								auto& sprite = Engine::Registry().get<Sprite>(cstats.currentHealthBar);
								cstats.healthBarOffset += (sprite.size.x - (BAR_START_SIZE * (cstats.currentHealth / cstats.maxHealth))) / 2;
								sprite.size.x = BAR_START_SIZE * (cstats.currentHealth / cstats.maxHealth);
							}
						}
						if (Engine::Registry().has<Projectile>(*it)) {
							auto& projectile = Engine::Registry().get<Projectile>(*it);				
							auto& pspawner = Engine::Registry().get<PlightParticleSpawner>(entity);
							pspawner.active = true;
							projectile.destroy = true;

						    cstats.currentHealth -= projectile.projectileDamage;	

							if (cstats.currentHealth <= 0.f) {
								cstats.currentHealth = 0.f;
							}

							auto& sprite = Engine::Registry().get<Sprite>(cstats.currentHealthBar);
							cstats.healthBarOffset += (sprite.size.x - (BAR_START_SIZE * (cstats.currentHealth / cstats.maxHealth))) / 2;
							sprite.size.x = BAR_START_SIZE * (cstats.currentHealth / cstats.maxHealth);
						}
					}
					it++;
				}
						
			}
		

			if (cstats.currentHealth <= 0.f) {
				cstats.currentHealth = 0.f;
				character.dead = true;
			}
			
			if (cstats.currentTimer >= cstats.updateTimer) {
				if (character.dashing) {
					cstats.currentStamina -= STAMINA_FOR_DASHING_FRAME;
					if (cstats.currentStamina < STAMINA_FOR_DASHING_FRAME) {
						if (cstats.currentStamina < 0.f) {
							cstats.currentStamina = 0.f;
						}
						character.running = false;
						character.dashing = false;
						character.resting = true;

					}
					auto& sprite = Engine::Registry().get<Sprite>(cstats.currentStaminaBar);
					cstats.staminaBarOffset -= (sprite.size.x - (BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina))) / 2;
					sprite.size.x = BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina);
				}
				else {
					cstats.currentStamina += STAMINA_FOR_REGENERATING_FRAME / 2;
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
			characterCurrentHealthSprite.position.x -= cstats.healthBarOffset;

			characterBackgroundStaminaSprite.position.y = characterSprite.position.y + cstats.playerDistance;
			characterBackgroundStaminaSprite.position.x = characterSprite.position.x;

			characterCurrentStaminaSprite.position.y = characterSprite.position.y + cstats.playerDistance;
			characterCurrentStaminaSprite.position.x = characterSprite.position.x;
			characterCurrentStaminaSprite.position.x += cstats.staminaBarOffset;
		}
	}

}