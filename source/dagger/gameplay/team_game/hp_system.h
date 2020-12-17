#pragma once

#include "core/core.h"
#include "core/system.h"
#include "core/engine.h"
#include "core/graphics/sprite.h"

using namespace dagger;

namespace ancient_defenders {

    struct Health {
        Entity hpBar;
        
        Float32 currentHealth;
        Float32 maxHealth;

        static constexpr Float32 standardHP = 100.0f;
    };

    class HealthManagementSystem
        : public System
    {
    public:
        inline String SystemName() { return "Health Management System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        void OnEndOfFrame();
    };
}
