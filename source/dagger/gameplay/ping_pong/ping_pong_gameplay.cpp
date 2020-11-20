#include "ping_pong_gameplay.h"
#if defined(DAGGER_DEBUG)

#include "core/engine.h"

#include "gameplay/ping_pong/ping_pong_main.h"
#include "gameplay/ping_pong/player_scores.h"
#include "gameplay/ping_pong/pingpong_playerinput.h"


void ping_pong::PingPongGameplay::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&PingPongGameplay::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().connect<&PingPongGameplay::RenderToolMenu>(this);
}
void ping_pong::PingPongGameplay::Run()
{
}
void ping_pong::PingPongGameplay::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&PingPongGameplay::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().disconnect<&PingPongGameplay::RenderToolMenu>(this);
}
void ping_pong::PingPongGameplay::RenderToolMenu()
{
    if (ImGui::BeginMenu("Gameplay"))
    {
        if (ImGui::MenuItem("vsPlayer"))
        {
            vsPlayer = true;
        }

        ImGui::Separator();

        if (ImGui::MenuItem("vsAI"))
        {
            vsAI = true;
        }

        ImGui::EndMenu();
    }
}
void ping_pong::PingPongGameplay::OnEndOfFrame()
{
    if (vsPlayer)
    {
        PingPongPlayerInputSystem::AI = false;
        vsPlayer = false;
    }
    if (vsAI)
    {
        PingPongPlayerInputSystem::AI = true;
        vsAI = false;
    }
}
#endif //defined(DAGGER_DEBUG)