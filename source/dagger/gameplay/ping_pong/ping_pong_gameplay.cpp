#include "ping_pong_gameplay.h"
#if defined(DAGGER_DEBUG)

#include "core/engine.h"

#include "gameplay/ping_pong/ping_pong_main.h"
#include "gameplay/ping_pong/player_scores.h"
#include "gameplay/ping_pong/pingpong_playerinput.h"


bool ping_pong::PingPongGameplay::AI = false;
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
        AI = false;
        vsPlayer = false;
        Engine::Registry().clear();

        ping_pong::SetupWorld(Engine::Instance());
    }
    if (vsAI)
    {
        AI = true;
        vsAI = false;
        Engine::Registry().clear();

        ping_pong::SetupWorld(Engine::Instance());
    }
}
#endif //defined(DAGGER_DEBUG)