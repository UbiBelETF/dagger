#include "gravity.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/team_game/team_game_collisions.h"

using namespace dagger;

void team_game::GravitySystem::Run()
{
    Engine::Registry().view<Collider, Gravity, Transform>().each([&](Collider& collider_, Gravity& gravity_, Transform& transform_) 
        {
            if (collider_.hasGravity)
            {
                if (!collider_.canGoUp)
                {
                    gravity_.verticalCurrentSpeed = -70.f;
                }

                if (collider_.canGoDown)
                {
                    gravity_.verticalCurrentSpeed -= m_GravityAcceleration * Engine::DeltaTime();
                }

                transform_.position.y += gravity_.verticalCurrentSpeed * Engine::DeltaTime();
            }
            
        });
}
