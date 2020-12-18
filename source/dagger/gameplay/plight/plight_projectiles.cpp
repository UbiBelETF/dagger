#define _USE_MATH_DEFINES
#include "plight_projectiles.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include <cmath>
#include <math.h>

#include "gameplay/plight/plight_collisions.h"
#include "gameplay/plight/plight_aiming.h"

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

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = pos_;

    auto& projectile = reg.emplace<Projectile>(entity);

    projectile.projectileDamage = settings_.projectileDamage;
    projectile.projectileSpeed = settings_.projectileSpeed;
    projectile.angle = angle_;

    reg.emplace<PlightCollision>(entity);
}

void ProjectileSystem::Run()
{
    //Check for fire input 
    auto projectileSpawners = Engine::Registry().view<ProjectileSpawner, Transform, InputReceiver,PlightCrosshair>();
    for (auto& entity : projectileSpawners) {
        auto& projectileSys = projectileSpawners.get<ProjectileSpawner>(entity);
        auto& t = projectileSpawners.get<Transform>(entity);
        auto& input = projectileSpawners.get<InputReceiver>(entity);
        auto& crosshair = projectileSpawners.get<PlightCrosshair>(entity);
        if (projectileSys.active) {
            Float32 fire = input.Get("fire");
            if (EPSILON_NOT_ZERO(fire)) {

                auto& sprite = Engine::Registry().get<Sprite>(crosshair.crosshairSprite);

                Float32 x = crosshair.playerDistance * cos(crosshair.angle);
                Float32 y = crosshair.playerDistance * sin(crosshair.angle);
                
                Vector3 pos = {x+t.position.x,y+t.position.y,t.position.z};
                CreateProjectile(projectileSys.settings,pos,crosshair.angle);
            }
        }     
    }

    // update all projectiles
    Engine::Registry().view<Projectile, Transform, Sprite>().each([&](Projectile& projectile_, Transform& transform_, Sprite& sprite_)
        {
            projectile_.timeOfLiving -= Engine::DeltaTime();
            Float32 dx = projectile_.projectileSpeed * cos(projectile_.angle) * Engine::DeltaTime();
            Float32 dy = projectile_.projectileSpeed * sin(projectile_.angle) * Engine::DeltaTime();

            transform_.position.x += dx;
            transform_.position.y += dy;
               
        });
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
        if (p.timeOfLiving <= 0)
        {
            Engine::Registry().remove_all(entity);
        }
    }
}

