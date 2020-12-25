#define _USE_MATH_DEFINES
#include "plight_projectiles.h"
#include "plight_game_logic.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include <cmath>
#include <math.h>

#include "gameplay/plight/plight_collisions.h"
#include "gameplay/plight/plight_combat.h"
#include "gameplay/plight/plight_controller.h"
#include "gameplay/plight/plight_aiming.h"
#include "gameplay/plight/plight_particles.h"
#include "gameplay/plight/plight_physics.h"
#include "gameplay/plight/plight_melee.h"
#include "core/graphics/animations.h"

#include <algorithm>
#include <execution>


using namespace dagger;
using namespace plight;

void ProjectileSystem::SetupProjectileSystem(Entity entity_, const ProjectileSpawnerSettings& settings_, const ProjectileSpawnerSettings& bombSettings_)
{
    auto& reg = Engine::Registry();
    auto& projectileSys = reg.emplace<ProjectileSpawner>(entity_);
    projectileSys.settings = settings_;
	projectileSys.bombSettings = bombSettings_;
}

void ProjectileSystem::CreateProjectile(const ProjectileSpawnerSettings& settings_,Vector3 pos_,Float32 angle_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();

    auto& sprite = reg.emplace<Sprite>(entity);
    sprite.rotation = (angle_ * 180.) / M_PI - 45;
    AssignSprite(sprite, settings_.pSpriteName);
    sprite.position = pos_;

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = pos_;


    auto& col = reg.emplace<PlightCollision>(entity);
	col.size.x = 5;
	col.size.y = 5;
	reg.emplace<PlightCollisionInitiator>(entity);

    auto& projectile = reg.emplace<Projectile>(entity);

    projectile.projectileDamage = settings_.projectileDamage;
    projectile.projectileSpeed = settings_.projectileSpeed;
	projectile.isBomb = settings_.bombSpawner;
    projectile.angle = angle_;

	if (settings_.bombSpawner == false) {
		//Particle spawner for projectiles
		PlightParticleSpawnerSettings particle_settings;
		particle_settings.Setup(0.05f, { 6.f, 6.f }, { -0.25f, -0.25f }, { 0.25f, 0.15f },
			{ 1.f,1.f,1.f,1 }, { 1.f,1.f,1.f,1 }, "dust", 0.02f, 0.4f, 0.4f);
		PlightParticleSystem::SetupParticleSystem(entity, particle_settings);
	}
	else {
		//Particle spawner for bombs
		PlightParticleSpawnerSettings particle_settings;
		particle_settings.Setup(0.1f, { 10.f, 10.f }, { -0.0f, -0.0f }, { 0.0f, 0.0f },
			{ 1.f,1.f,1.f,1 }, { 1.f,1.f,1.f,1 }, "explosion", 0.15f, 0.4f, 0.3f);
		PlightParticleSystem::SetupParticleSystem(entity, particle_settings);
	}
}

void ProjectileSystem::Run()
{
	auto viewLS1 = Engine::Registry().view<PlightIntro>();
	auto it = viewLS1.begin();
	auto& pin = viewLS1.get<PlightIntro>(*it);

	if (pin.IsFinished()) 
	{
		//Check for fire input 
		auto projectileSpawners = Engine::Registry().view<ProjectileSpawner, Transform, InputReceiver, PlightCrosshair, CombatStats,PlightCharacterController>();
		for (auto& entity : projectileSpawners) {
			auto& projectileSys = projectileSpawners.get<ProjectileSpawner>(entity);
			auto& t = projectileSpawners.get<Transform>(entity);
			auto& input = projectileSpawners.get<InputReceiver>(entity);
			auto& character = projectileSpawners.get<PlightCharacterController>(entity);
			auto& crosshair = projectileSpawners.get<PlightCrosshair>(entity);
			auto& cstats = projectileSpawners.get<CombatStats>(entity);
			if (character.dead) {
				continue;
			}

			if (projectileSys.active) {
				Float32 fire = input.Get("fire");
				Float32 bomb = input.Get("bomb");
				if (EPSILON_NOT_ZERO(bomb) || EPSILON_NOT_ZERO(fire)) {
					auto& weapon = Engine::Registry().get<Weapon>(character.weaponSprite);
					if (character.attacking) {
						if (!weapon.attacking) {
							auto& weapon_sprt = Engine::Registry().get<Sprite>(character.weaponSprite);
							AssignSprite(weapon_sprt, character.weaponSpriteName);
						}
						character.attacking = false;

					}
				}
				if (EPSILON_NOT_ZERO(bomb) && cstats.currentStamina >= BOMB_COST) {
					cstats.currentStamina -= BOMB_COST;

					auto& sprite = Engine::Registry().get<Sprite>(cstats.currentStaminaBar);
					cstats.staminaBarOffset -= (sprite.size.x - (BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina))) / 2;
					sprite.size.x = BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina);

					Float32 x = crosshair.playerDistance * cos(crosshair.angle);
					Float32 y = crosshair.playerDistance * sin(crosshair.angle);

					Vector3 pos = { x + t.position.x,y + t.position.y,t.position.z };
					CreateProjectile(projectileSys.bombSettings, pos, crosshair.angle);

				}
				else if (EPSILON_NOT_ZERO(fire)) {
				
					if (cstats.currentStamina >= PROJECTILE_COST) {
						cstats.currentStamina -= PROJECTILE_COST;

						auto& sprite = Engine::Registry().get<Sprite>(cstats.currentStaminaBar);
						cstats.staminaBarOffset -= (sprite.size.x - (BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina))) / 2;
						sprite.size.x = BAR_START_SIZE * (cstats.currentStamina / cstats.maxStamina);

						Float32 x = crosshair.playerDistance * cos(crosshair.angle);
						Float32 y = crosshair.playerDistance * sin(crosshair.angle);

						Vector3 pos = { x + t.position.x,y + t.position.y,t.position.z };
						CreateProjectile(projectileSys.settings, pos, crosshair.angle);
					}
				}
			}
		}

		// update all projectiles
		Engine::Registry().view<Projectile, Transform, Sprite,PlightCollision,PlightParticleSpawner>().each([&](Projectile& projectile_, Transform& transform_, Sprite& sprite_,PlightCollision& col_,PlightParticleSpawner& pspawner_)
		{
			projectile_.timeOfLiving -= Engine::DeltaTime();
			if (projectile_.timeOfLiving <= 0) {
				if (projectile_.isBomb) {
					pspawner_.active = true;
					projectile_.activated = true;
					col_.size.x = projectile_.bombRadius;
					col_.size.y = projectile_.bombRadius;
				}
				else {
					projectile_.destroy = true;
				}
				
			}
			if (col_.colided) {
				auto it = col_.colidedWith.begin();
				while (it != col_.colidedWith.end()) {
					if (Engine::Registry().has<PhysicsObject>(*it)) {
						auto& physics_object = Engine::Registry().get<PhysicsObject>(*it);
						if (physics_object.is_static) {
							pspawner_.active = true;
							if (projectile_.isBomb) {
								col_.size.x = projectile_.bombRadius;
								col_.size.y = projectile_.bombRadius;
								projectile_.activated = true;
							}	
							else {
								projectile_.displayingParticles = true;
							}

						}
					}
					it++;
				}
			}

			Float32 dx = projectile_.projectileSpeed * cos(projectile_.angle) * Engine::DeltaTime();
			Float32 dy = projectile_.projectileSpeed * sin(projectile_.angle) * Engine::DeltaTime();

			transform_.position.x += dx;
			transform_.position.y += dy;

		});
	}
}

void ProjectileSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&ProjectileSystem::OnEndOfFrame>(this);
}

void ProjectileSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&ProjectileSystem::OnEndOfFrame>(this);
}

void ProjectileSystem::OnEndOfFrame()
{
   //Need to change this part  to check if projectile colided with something , if yes destroy it

    auto projectiles = Engine::Registry().view<Projectile>();

    for (auto& entity : projectiles)
    {
        auto& p = projectiles.get<Projectile>(entity);
		if (p.displayingParticles) {
			Engine::Registry().remove_if_exists<Sprite>(entity);	
			auto& particle_spawner = Engine::Registry().get<PlightParticleSpawner>(entity);
			if (!particle_spawner.active) {
				p.destroy = true;
			}
		}
        if (p.destroy)
        {
            Engine::Registry().remove_all(entity);
        }
    }

	auto& chrView = Engine::Registry().view<PlightCharacterController>();
	for (auto entity : chrView) {
		auto& controler = chrView.get<PlightCharacterController>(entity);
		auto& weapon = Engine::Registry().get<Weapon>(controler.weaponSprite);
		if (!controler.attacking) {
			if (weapon.attacking) {
				auto& anim = Engine::Registry().get<Animator>(controler.weaponSprite);
				AnimatorStop(anim);
				Engine::Registry().remove_if_exists<Animator>(controler.weaponSprite);
				auto& sprite = Engine::Registry().get<Sprite>(controler.weaponSprite);
				AssignSprite(sprite, controler.weaponSpriteName);
				weapon.attacking = false;
			}
		}

	}
}

