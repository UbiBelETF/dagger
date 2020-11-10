#pragma once

#include <core/system.h>

using namespace dagger;

struct ImpactFlash
{
    float flashingRate{ 4.0f };
    bool isFlashing{ false };
    bool isWindingDown{ false };
};

class ImpactFlashSystem
    : public System
{
public:
    inline String SystemName() { return "Impact Flash System"; }

    void Run() override;
};
