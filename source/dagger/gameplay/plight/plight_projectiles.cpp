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

#include <algorithm>
#include <execution>


using namespace dagger;
using namespace plight;

void ProjectileSystem::SetupProjectileSystem(Entity entity_, const ProjectileSpawnerSettings& settings_)
{
    auto& reg = Engine::Registry();
    auto& projectileSys = reg.emplace<ProjectileSpawner>(entity_);
    projectileSys.settings = settings_;
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


    reg.emplace<PlightCollision>(entity);
	reg.emplace<PlightCollisionInitiator>(entity);

    auto& projectile = reg.emplace<Projectile>(entity);

    projectile.projectileDamage = settings_.projectileDamage;
    projectile.projectileSpeed = settings_.projectileSpeed;
    projectile.angle = angle_;

	//Particle spawner for colliding with walls
	PlightParticleSpawnerSettings particle_settings;
	particle_settings.Setup(0.05f, { 6.f, 6.f }, { -0.25f, -0.25f }, { 0.25f, 0.15f },
		{ 1.f,1.f,1.f,1 }, { 1.f,1.f,1.f,1 }, "dust", 0.02f, 0.4f, 0.4f);
	PlightParticleSystem::SetupParticleSystem(entity, particle_settings);

}

void ProjectileSystem::Run()
{
	auto viewLS1 = Engine::Registry().view<PlightIntro>();
	auto it = viewLS1.begin();
	auto& pin = viewLS1.get<PlightIntro>(*it);

	if (pin.IsFinished()) 
	{
		//Check for fire input 
		auto projectileSpawners = Engine::Registry().view<ProjectileSpawner, Transform, InputReceiver, PlightCrosshair, CombatStats>();
		for (auto& entity : projectileSpawners) {
			auto& projectileSys = projectileSpawners.get<ProjectileSpawner>(entity);
			auto& t = projectileSpawners.get<Transform>(entity);
			auto& input = projectileSpawners.get<InputReceiver>(entity);
			auto& crosshair = projectileSpawners.get<PlightCrosshair>(entity);
			auto& cstats = projectileSpawners.get<CombatStats>(entity);

			if (projectileSys.active) {
				Float32 fire = input.Get("fire");
				if (EPSILON_NOT_ZERO(fire)) {
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
				projectile_.destroy = true;
			}
			if (col_.colided) {
				auto it = col_.colidedWith.begin();
				while (it != col_.colidedWith.end()) {
					if (Engine::Registry().has<PhysicsObject>(*it)) {
						auto& physics_object = Engine::Registry().get<PhysicsObject>(*it);
						if (physics_object.is_static) {
							pspawner_.active = true;
							projectile_.displayingParticles = true;
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
}

