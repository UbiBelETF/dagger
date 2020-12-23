#pragma once

#include "core/system.h"
#include "core/core.h"
    
using namespace dagger;

namespace plight
{

struct PlightParticle
{
    Vector2 scaleSpeed;
    Vector3 positionSpeed;
    Vector4 colorSpeed;
    Float32 timeOfLiving = 1.f;
};

struct PlightParticleSpawnerSettings
{
    Float32 timeToNewParticle = 1.f;
    Vector2 pSize;
    Vector2 pSpeedMin;
    Vector2 pSpeedMax;
    ColorRGBA pColorMin{ 1.0f, 1.0f, 1.0f, 1.0f };
    ColorRGBA pColorMax{ 1.0f, 1.0f, 1.0f, 1.0f };
    String pSpriteName;
    Float32 pTimeOfLiving = 1.f;
    Float32 pScaleSpeed = 0.05f;

    Float32 spawnDuration{ 2.f };
    Float32 currentSpawnDuration{ 0.f };

    void Setup(Float32 timeToNewParticle_, Vector2 pSize_, Vector2 pSpeedMin_, Vector2 pSpeedMax_,
        ColorRGBA pColorMin_ = { 1.0f, 1.0f, 1.0f, 1.0f }, ColorRGBA pColorMax_ = { 1.0f, 1.0f, 1.0f, 1.0f },
        String pSpriteName_ = "EmptyWhitePixel", Float32 pScaleSpeed_ = 0.05f , Float32 spawnDuration_ = 2.f, Float32 pTimeOfLiving_ = 1.f)
    {
        timeToNewParticle = timeToNewParticle_;
        pSize = pSize_;
        pSpeedMin = pSpeedMin_;
        pSpeedMax = pSpeedMax_;
        pSpriteName = pSpriteName_;
        pColorMin = pColorMin_;
        pColorMax = pColorMax_;
        pScaleSpeed = pScaleSpeed_;
        spawnDuration = spawnDuration_;
        pTimeOfLiving = pTimeOfLiving_;
    }
};

struct PlightParticleSpawner
{
    Bool active = false;
    Float32 timer = 0.f;

    PlightParticleSpawnerSettings settings;
};

class PlightParticleSystem
    : public System
{
    inline String SystemName() { return "Plight Particle System"; }

    void SpinUp() override;
    void Run() override;
    void WindDown() override;


public:
    static void SetupParticleSystem(Entity entity_, const PlightParticleSpawnerSettings& settings_);

private:
    void OnEndOfFrame();

    void CreateParticle(const PlightParticleSpawnerSettings& settings_, Vector3 pos_);
};
}