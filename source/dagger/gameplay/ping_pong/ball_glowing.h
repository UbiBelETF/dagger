#pragma once
#include "core/system.h"

using namespace dagger;

struct BallGlowing
{
    ColorRGBA color_for_ball;
    bool isColored{ false };
     
};

class BallGlowingSystem
    : public System
{
public:
    inline String SystemName() { return "Ball Glowing System"; }

    void Run() override;
};
