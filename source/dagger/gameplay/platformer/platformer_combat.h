#pragma once
#include "core/system.h"
#include "core/engine.h"
#include "core/core.h"
#include "core/graphics/sprite.h"

#include "gameplay/platformer/parallax.h"

using namespace dagger;

namespace platformer {
    struct CharacterHealth
    {
        bool frameDmg{ false }; //dmg is calculated only one time during a single colision 

        Float32 maxHealth{ 100 };
        Float32 currentHealth{ 100 };

        Entity backgroundBar;
        Entity currentHealthBar;
    };

    class PlatformerCombatSystem : public System
    {
    public:
        inline String SystemName() { return "Character Combat System"; }

        void Run() override;
    };
}
