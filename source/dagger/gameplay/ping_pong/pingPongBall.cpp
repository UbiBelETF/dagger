#include "pingPongBall.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include <algorithm>
#include <execution>

#include <GLFW/glfw3.h>

using namespace dagger;
using namespace PingPong;

void PingPongBallSystem::Run()
{
    auto view = Engine::Registry().view<PingPongBall, Transform>();
    for(auto entity: view) 
    {
        auto &t = view.get<Transform>(entity);
        auto &ball = view.get<PingPongBall>(entity);

        if (ball.collided)
        {
            // get back for 1 frame 
            Float32 dt = Engine::DeltaTime();
            t.position -= (ball.speed * dt);

            if (std::abs(ball.collisionSides.x) > 0)
            {
                ball.speed.x *= -1;
            }
            
            if (std::abs(ball.collisionSides.y) > 0)
            {
                ball.speed.y *= -1;
            }

            ball.collided = false;
        }

        t.position += (ball.speed * Engine::DeltaTime());
    }
}
