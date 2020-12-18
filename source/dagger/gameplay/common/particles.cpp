#include "particles.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"

#include <algorithm>
#include <execution>

using namespace dagger;
using namespace common_res;

void ParticleSystem::SetupParticleSystem(Entity entity_, const ParticleSpawnerSettings& settings_)
{
    auto& reg = Engine::Registry();
    auto& particleSys = reg.emplace<ParticleSpawner>(entity_);
    particleSys.settings = settings_;
}

// get rand float value from 0 to 1
Float32 particle_getRand()
{
    return ((double) rand() / (RAND_MAX));
}

void ParticleSystem::CreateParticle(const ParticleSpawnerSettings& settings_, Vector3 pos_)
{
    auto& reg = Engine::Registry();
    auto entity = reg.create();
    
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSprite(sprite, settings_.pSpriteName);
    sprite.size = settings_.pSize;

    Vector4 randColorVal;
    randColorVal.r = glm::mix(settings_.pColorMin.r, settings_.pColorMax.r, particle_getRand());
    randColorVal.g = glm::mix(settings_.pColorMin.g, settings_.pColorMax.g, particle_getRand());
    randColorVal.b = glm::mix(settings_.pColorMin.b, settings_.pColorMax.b, particle_getRand());
    randColorVal.a = glm::mix(settings_.pColorMin.a, settings_.pColorMax.a, particle_getRand());
    sprite.color = randColorVal;

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = pos_;

    auto& particle = reg.emplace<Particle>(entity);
    
    Vector2 randSpeedVal;
    randSpeedVal.x = glm::mix(settings_.pSpeedMin.x, settings_.pSpeedMax.x, particle_getRand());
    randSpeedVal.y = glm::mix(settings_.pSpeedMin.y, settings_.pSpeedMax.y, particle_getRand());
    particle.positionSpeed = {randSpeedVal.x, randSpeedVal.y, 0.f};

    particle.colorSpeed = {0.f,0.f,0.f,-0.01f};

    Float32 randAddition = 0.1f * (1 - 2 * (rand() % 2));
    particle.scaleSpeed = settings_.pSize * randAddition;
}

void ParticleSystem::Run()
{
    // update all particle generators
    {
        auto particles = Engine::Registry().view<ParticleSpawner, Transform>();

        for (auto& entity : particles)
        {
            auto& particleSys_ = particles.get<ParticleSpawner>(entity);
            auto& t = particles.get<Transform>(entity);
            if (particleSys_.active)
            {
                particleSys_.timer -= Engine::DeltaTime();
                if (particleSys_.timer < 0)
                {
                    particleSys_.timer = particleSys_.settings.timeToNewParticle;

                    CreateParticle(particleSys_.settings, t.position);
                }
            }
        }
    }

    // update all particles
    Engine::Registry().view<Particle, Transform, Sprite>().each([&](Particle& particle_, Transform& transform_, Sprite& sprite_)
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

void ParticleSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&ParticleSystem::OnEndOfFrame>(this);
}

void ParticleSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&ParticleSystem::OnEndOfFrame>(this);
}

void ParticleSystem::OnEndOfFrame()
{
    auto particles = Engine::Registry().view<Particle>();

    for (auto& entity : particles)
    {
        auto& p = particles.get<Particle>(entity);
        if (p.timeOfLiving <= 0)
        {
            Engine::Registry().remove_all(entity);
        }
    }
}

