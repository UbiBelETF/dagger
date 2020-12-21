#pragma once

#include "core/system.h"
#include "core/core.h"


#define PROJECTILE_COST 5.f

using namespace dagger;

namespace plight
{

    struct Projectile
    {
        Float32 projectileSpeed;
        Float32 projectileDamage;
        Float32 angle;
        Float32 timeOfLiving = 2.f;

        bool destroy = false;
    };

    struct ProjectileSpawnerSettings
    {
        Float32 projectileSpeed = 100.f;
        Float32 projectileDamage = 5.f;
        String pSpriteName;

        void Setup(Float32 projectileSpeed_, Float32 projectileDamage_,
            String pSpriteName_ = "EmptyWhitePixel")
        {
            projectileSpeed = projectileSpeed_;
            projectileDamage = projectileDamage_;
            pSpriteName = pSpriteName_;
            
        }
    };

    class ProjectileSystem
        : public System
    {
        inline String SystemName() { return "Projectile System"; }

        void SpinUp() override;
        void Run() override;
        void WindDown() override;

        struct ProjectileSpawner
        {
            Bool active = true;
            Float32 timer = 0.f;

            ProjectileSpawnerSettings settings;
        };

    public:
        static void SetupProjectileSystem(Entity entity_, const ProjectileSpawnerSettings& settings_);

    private:
        void OnEndOfFrame();

        void CreateProjectile(const ProjectileSpawnerSettings& settings_, Vector3 pos_,Float32 angle_);
    };
}