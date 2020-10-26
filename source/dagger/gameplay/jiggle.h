#pragma once

#include "core/system.h"
#include "core/core.h"
    
using namespace dagger;

class JiggleSystem
    : public System
{
    inline String SystemName() { return "Jiggle System"; }

    void Run() override;
};