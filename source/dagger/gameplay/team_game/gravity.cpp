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
            if (!collider_.canGoUp)
            {
                gravity_.verticalCurrentSpeed = -100.f;
            }
            if (collider_.canGoDown)
            {       
                if (abs(gravity_.verticalCurrentSpeed) > gravity_.terminalVelocity)
                {
                    gravity_.verticalCurrentSpeed = - gravity_.terminalVelocity;
                }
                
                transform_.position.y += gravity_.verticalCurrentSpeed * Engine::DeltaTime();
                
                gravity_.verticalCurrentSpeed -= m_GravityAcceleration * Engine::DeltaTime();
            }
            else
            {
                gravity_.verticalCurrentSpeed = 0.f;
            } 
        });
}
