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
            if (collider_.canGoDown)
            {                
                if (gravity_.terminalVelocity < m_GravityStrength * gravity_.airTime)
                {
                    transform_.position.y -= gravity_.terminalVelocity;
                }
                else
                {
                    transform_.position.y -= m_GravityStrength * gravity_.airTime;
                }
                gravity_.airTime += Engine::DeltaTime();
                Logger::trace("My G speed is:{}", m_GravityStrength * gravity_.airTime);
            }
            else
            {
                gravity_.airTime = 0.f;
            } 
        });
}
