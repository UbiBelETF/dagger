#include "pingPongBall.h"

#include "core/engine.h"
#include "gameplay/transform.h"

#include <algorithm>
#include <execution>

#include <GLFW/glfw3.h>

using namespace dagger;

void PingPongBallSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PingPongBallSystem::OnKeyboardEvent>(this);
}

void PingPongBallSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PingPongBallSystem::OnKeyboardEvent>(this);
}

void PingPongBallSystem::Run()
{
    auto view = Engine::Registry().view<PingPongBall, Transform, ControllerMapping>();
    for(auto entity: view) 
    {
        auto &t = view.get<Transform>(entity);
        auto &ball = view.get<PingPongBall>(entity);
        auto &ctrl = view.get<ControllerMapping>(entity);

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

        if (ctrl.input.y > 0)
        {
            ball.speed.y *= -1;
            ctrl.input.y = 0;
        }
    }
}

void PingPongBallSystem::OnKeyboardEvent(KeyboardEvent kEvent)
{
    Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
    {
        if (kEvent.key == ctrl_.key /*GLFW_KEY_W*/ && kEvent.action == GLFW_PRESS)
        {
            ctrl_.input.y = 1;
        }
    });
}
