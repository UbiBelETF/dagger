#include "pingpong_tools.h"

#if defined(DAGGER_DEBUG)

#include "core/engine.h"

#include "gameplay/ping_pong/ping_pong_main.h"
#include "gameplay/ping_pong/player_scores.h"

using namespace dagger;
using namespace ping_pong;

void PingPongTools::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&PingPongTools::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().connect<&PingPongTools::RenderToolMenu>(this);
}

void PingPongTools::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&PingPongTools::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().disconnect<&PingPongTools::RenderToolMenu>(this);
}

void PingPongTools::RenderToolMenu()
{
    if (ImGui::BeginMenu("Ping Pong"))
    {
        if (ImGui::MenuItem("Restart"))
        {
            dagger::Engine::ToggleSystemsPause(false);
            m_RestartGame = true;
        }

        ImGui::Separator();

        if (ImGui::MenuItem("Generate ball"))
        {
            m_GenerateBall = true;
        }

        ImGui::EndMenu();
    }
}

void PingPongTools::Run()
{
}

void PingPongTools::OnEndOfFrame()
{
    if (m_RestartGame)
    {
        m_RestartGame = false;
        Engine::Registry().clear();

        ping_pong::SetupWorld(Engine::Instance());
    }

    if (m_GenerateBall)
    {
        m_GenerateBall = false;

        ping_pong::CreatePingPongBall(PlayerScoresSystem::GetTileSize(), ColorRGBA(1, 1, 1, 1), 
            { (1 - 2 * (rand() % 2)) * (rand()%10 + 4),rand()%10 + 4,0 },   { 0,0,0 });
    }
}

#endif //defined(DAGGER_DEBUG)