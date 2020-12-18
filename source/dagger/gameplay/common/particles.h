#pragma once

#include "core/system.h"
#include "core/core.h"
    
using namespace dagger;

namespace common_res
{

struct Particle
{
    Vector2 scaleSpeed;
    Vector3 positionSpeed;
    Vector4 colorSpeed;
    Float32 timeOfLiving = 1.f;
};

struct ParticleSpawnerSettings
{
    Float32 timeToNewParticle = 1.f;
    Vector2 pSize;
    Vector2 pSpeedMin;
    Vector2 pSpeedMax;
    ColorRGBA pColorMin{ 1.0f, 1.0f, 1.0f, 1.0f };
    ColorRGBA pColorMax{ 1.0f, 1.0f, 1.0f, 1.0f };
    String pSpriteName;

    void Setup(Float32 timeToNewParticle_, Vector2 pSize_, Vector2 pSpeedMin_, Vector2 pSpeedMax_, 
                ColorRGBA pColorMin_ = { 1.0f, 1.0f, 1.0f, 1.0f }, ColorRGBA pColorMax_ = { 1.0f, 1.0f, 1.0f, 1.0f }, 
                String pSpriteName_ = "EmptyWhitePixel")
    {
        timeToNewParticle = timeToNewParticle_;
        pSize = pSize_;
        pSpeedMin = pSpeedMin_;
        pSpeedMax = pSpeedMax_;
        pSpriteName = pSpriteName_;
        pColorMin = pColorMin_;
        pColorMax = pColorMax_;
    }
};

class ParticleSystem
    : public System
{
    inline String SystemName() { return "Particle System"; }

    void SpinUp() override;
    void Run() override;
    void WindDown() override;

    struct ParticleSpawner
    {
        Bool active = true;
        Float32 timer = 0.f;

        ParticleSpawnerSettings settings;
    };

public:
    static void SetupParticleSystem(Entity entity_, const ParticleSpawnerSettings& settings_);

private:
    void OnEndOfFrame();

    void CreateParticle(const ParticleSpawnerSettings& settings_, Vector3 pos_);
};
}