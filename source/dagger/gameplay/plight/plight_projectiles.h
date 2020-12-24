#pragma once

#include "core/system.h"
#include "core/core.h"


#define PROJECTILE_COST 5.f
#define BOMB_COST 70.f

using namespace dagger;

namespace plight
{

    struct Projectile
    {
        Float32 projectileSpeed;
        Float32 projectileDamage;
        Float32 angle;
        Float32 timeOfLiving = 2.f;
        
        bool isBomb = false;
        Float32 bombRadius = 100.f;

        bool destroy = false;
        bool displayingParticles = false;
        bool bombCollisionDetected = false;
        bool activated = false;
    };

    struct ProjectileSpawnerSettings
    {
        Float32 projectileSpeed = 100.f;
        Float32 projectileDamage = 5.f;
        String pSpriteName;
        bool bombSpawner = false;

        void Setup(Float32 projectileSpeed_, Float32 projectileDamage_,
            String pSpriteName_ = "EmptyWhitePixel",bool bombSpawner_ = false)
        {
            projectileSpeed = projectileSpeed_;
            projectileDamage = projectileDamage_;
            pSpriteName = pSpriteName_;
            bombSpawner = bombSpawner_;
        }
    };
    struct ProjectileSpawner
    {
        Bool active = true;
        Float32 timer = 0.f;

        ProjectileSpawnerSettings settings;
        ProjectileSpawnerSettings bombSettings;
    };

    class ProjectileSystem
        : public System
    {
        inline String SystemName() { return "Projectile System"; }

        void SpinUp() override;
        void Run() override;
        void WindDown() override;

        

    public:
        static void SetupProjectileSystem(Entity entity_, const ProjectileSpawnerSettings& settings_,const ProjectileSpawnerSettings& bombSettings_);

    private:
        void OnEndOfFrame();

        void CreateProjectile(const ProjectileSpawnerSettings& settings_, Vector3 pos_,Float32 angle_);
    };
}