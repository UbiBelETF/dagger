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
#include "gameplay/plight/plight_fields.h"





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
						auto& otherCol = Engine::Registry().get<PlightCollision>(*it);
						auto& otherTransform = Engine::Registry().get<Transform>(*it);
						if (Engine::Registry().has<Weapon>(*it)) {
							if (cstats.currentTimer >= cstats.updateTimer) {
								
								auto& weapon = Engine::Registry().get<Weapon>(*it);
							if (weapon.attacking) {
								auto& pspawner = Engine::Registry().get<PlightParticleSpawner>(entity);
								pspawner.active = true;
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

							if (projectile.isBomb) {
								if (!projectile.displayingParticles) {
									if (!projectile.activated) {
										auto& bomb_particles = Engine::Registry().get<PlightParticleSpawner>(*it);
										bomb_particles.active = true;
										otherCol.size.x = projectile.bombRadius;
										otherCol.size.y = projectile.bombRadius;
										projectile.activated = true;
									}									
								}
								else {
									if (!cstats.healing) {
										Float32 dmg = projectile.projectileDamage;
										Float32 dist = getDistance(t.position.x, t.position.y, otherTransform.position.x, otherTransform.position.y);
										if (dist > 0) {
											dmg = projectile.projectileDamage * 1 / (dist / 15);
											if (dmg > projectile.projectileDamage) {
												dmg = projectile.projectileDamage;
											}
										}
										cstats.currentHealth -= dmg;
									}
									
								}
							}
							else if(!projectile.destroy){
								cstats.currentHealth -= projectile.projectileDamage;
								projectile.destroy = true;
							}


								if (cstats.currentHealth <= 0.f) {
									cstats.currentHealth = 0.f;
								}

								auto& sprite = Engine::Registry().get<Sprite>(cstats.currentHealthBar);
								cstats.healthBarOffset += (sprite.size.x - (BAR_START_SIZE * (cstats.currentHealth / cstats.maxHealth))) / 2;
								sprite.size.x = BAR_START_SIZE * (cstats.currentHealth / cstats.maxHealth);				
						}
						if (Engine::Registry().has<DefenseField>(entity)){			
							auto& defenseField = Engine::Registry().get<DefenseField>(entity);
							if (defenseField.defenseFieldE == *it) {
								if (cstats.healing) {
									if (!defenseField.initialDone) {
										cstats.currentHealth += defenseField.initialHealthReturn;
										if (cstats.currentHealth > cstats.maxHealth) {
											cstats.currentHealth = cstats.maxHealth;
										}
										cstats.currentStamina -= defenseField.initialStaminaCost;
										if (cstats.currentStamina > cstats.maxStamina) {
											cstats.currentStamina = cstats.maxStamina;
										}
										defenseField.initialDone = true;
									}
									if (cstats.currentStamina < STAMINA_FOR_HEALING_FRAME) {
										it++;
										continue;
									}
									cstats.currentStamina -= STAMINA_FOR_HEALING_FRAME;
									if (cstats.currentStamina < 0.f) {
										cstats.currentStamina = 0.f;
									}
									auto& sprite = Engine::Registry().get<Sprite>(cstats.currentStaminaBar);
									cstats.staminaBarOffset -= (sprite.size.x - (BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina))) / 2;
									sprite.size.x = BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina);

									cstats.currentHealth += STAMINA_FOR_HEALING_FRAME/2;
									if (cstats.currentHealth > cstats.maxHealth) {
										cstats.currentHealth = cstats.maxHealth;
									}
									auto& sprite2 = Engine::Registry().get<Sprite>(cstats.currentHealthBar);
									cstats.healthBarOffset -= std::abs((sprite2.size.x - (BAR_START_SIZE * (cstats.currentHealth / cstats.maxHealth))) / 2);
									sprite2.size.x = BAR_START_SIZE * (cstats.currentHealth / cstats.maxHealth);
								}
							}
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
				else if(!cstats.healing || cstats.currentStamina < STAMINA_FOR_HEALING_FRAME){
					cstats.currentStamina += STAMINA_FOR_REGENERATING_FRAME;
					if (cstats.currentStamina > 100) {
						cstats.currentStamina = 100;
					}
						auto& sprite = Engine::Registry().get<Sprite>(cstats.currentStaminaBar);
						cstats.staminaBarOffset += std::abs((sprite.size.x - (BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina))) / 2);
						sprite.size.x = BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina);
					
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

void plight::PlightCombatSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&PlightCombatSystem::OnEndOfFrame>(this);
}

void plight::PlightCombatSystem::SpinUp()
{
	Engine::Dispatcher().sink<NextFrame>().connect<&PlightCombatSystem::OnEndOfFrame>(this);
}

void plight::PlightCombatSystem::OnEndOfFrame()
{
	auto view = Engine::Registry().view<Projectile>();
	for (auto entity : view) {
		auto& projectile = Engine::Registry().get<Projectile>(entity);
		if (projectile.isBomb) {
			if (projectile.activated) {
				if (!projectile.displayingParticles) {
					projectile.displayingParticles = true;
				}
				else {
					Engine::Registry().remove_if_exists<PlightCollision>(entity);
				}
			}
		}
	}
}

Float32 plight::PlightCombatSystem::getDistance(Float32 x1, Float32 y1, Float32 x2, Float32 y2)
{
		// Calculating distance 
		return sqrt(pow(x2 - x1, 2) +
			pow(y2 - y1, 2) * 1.0f);
	
}
