#include "plight_particles.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"

#include <algorithm>
#include <execution>

using namespace dagger;
using namespace plight;

void PlightParticleSystem::SetupParticleSystem(Entity entity_, const PlightParticleSpawnerSettings& settings_)
{
    auto& reg = Engine::Registry();
    auto& particleSys = reg.emplace<PlightParticleSpawner>(entity_);
    particleSys.settings = settings_;
}

// get rand float value from 0 to 1
Float32 PlightParticle_getRand()
{
    return ((double) rand() / (RAND_MAX));
}

void PlightParticleSystem::CreateParticle(const PlightParticleSpawnerSettings& settings_, Vector3 pos_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, settings_.pSpriteName);
    sprite.size = settings_.pSize;

    Vector4 randColorVal;
    randColorVal.r = glm::mix(settings_.pColorMin.r, settings_.pColorMax.r, PlightParticle_getRand());
    randColorVal.g = glm::mix(settings_.pColorMin.g, settings_.pColorMax.g, PlightParticle_getRand());
    randColorVal.b = glm::mix(settings_.pColorMin.b, settings_.pColorMax.b, PlightParticle_getRand());
    randColorVal.a = glm::mix(settings_.pColorMin.a, settings_.pColorMax.a, PlightParticle_getRand());
    sprite.color = randColorVal;

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = pos_;

    auto& particle = reg.emplace<PlightParticle>(entity);
    
    Vector2 randSpeedVal;
    randSpeedVal.x = glm::mix(settings_.pSpeedMin.x, settings_.pSpeedMax.x, PlightParticle_getRand());
    randSpeedVal.y = glm::mix(settings_.pSpeedMin.y, settings_.pSpeedMax.y, PlightParticle_getRand());
    particle.positionSpeed = {randSpeedVal.x, randSpeedVal.y, 0.f};

    particle.colorSpeed = {0.f,0.f,0.f,-0.01f};
    if (settings_.includeScaleSpeed) {
        Float32 randAddition = 0.1f * (1 - 2 * (rand() % 2));
        particle.scaleSpeed = settings_.pSize * randAddition;
    }
    else {
        particle.scaleSpeed = {0.f,0.f};
    }
    
}

void PlightParticleSystem::Run()
{
    // update all particle generators
    {
        auto particles = Engine::Registry().view<PlightParticleSpawner, Transform>();

        for (auto& entity : particles)
        {
            auto& particleSys_ = particles.get<PlightParticleSpawner>(entity);
            auto& t = particles.get<Transform>(entity);
            if (particleSys_.active)
            {
                particleSys_.timer -= Engine::DeltaTime();
                if (particleSys_.timer < 0)
                {
                    particleSys_.timer = particleSys_.settings.timeToNewParticle;

                    CreateParticle(particleSys_.settings, t.position);
                }

                particleSys_.settings.currentSpawnDuration += Engine::DeltaTime();
                if (particleSys_.settings.currentSpawnDuration >= particleSys_.settings.spawnDuration) {
                    particleSys_.settings.currentSpawnDuration = 0;
                    particleSys_.active = false;
                }
            }
        }
    }

    // update all particles
    Engine::Registry().view<PlightParticle, Transform, Sprite>().each([&](PlightParticle& particle_, Transform& transform_, Sprite& sprite_)
    {
        if (particle_.timeOfLiving > 0)
        {
            particle_.timeOfLiving -= Engine::DeltaTime();
            transform_.position += particle_.positionSpeed;
            sprite_.size += particle_.scaleSpeed;
            // might need to set color bounds
            sprite_.color += particle_.colorSpeed;
            // sprite_.color = glm::clamp(sprite_.color + particle_.colorSpeed, { 0, 0, 0, 0 }, { 1, 1, 1, 1 });
        }
    });
}

void PlightParticleSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&PlightParticleSystem::OnEndOfFrame>(this);
}

void PlightParticleSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&PlightParticleSystem::OnEndOfFrame>(this);
}

void PlightParticleSystem::OnEndOfFrame()
{
    auto particles = Engine::Registry().view<PlightParticle>();

    for (auto& entity : particles)
    {
        auto& p = particles.get<PlightParticle>(entity);
        if (p.timeOfLiving <= 0)
        {
            Engine::Registry().remove_all(entity);
        }
    }
}

