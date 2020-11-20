#include "racing_tools.h"

#if defined(DAGGER_DEBUG)

#include "core/engine.h"

#include "gameplay/racing/racing_main.h"

using namespace dagger;
using namespace racing_game;

void racing_game::RacingToolsSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&RacingToolsSystem::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().connect<&RacingToolsSystem::RenderToolMenu>(this);
}

void racing_game::RacingToolsSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&RacingToolsSystem::OnEndOfFrame>(this);

    Engine::Dispatcher().sink<ToolMenuRender>().disconnect<&RacingToolsSystem::RenderToolMenu>(this);
}

void racing_game::RacingToolsSystem::Run()
{
}

void racing_game::RacingToolsSystem::OnEndOfFrame()
{
    if (m_Restart)
    {
        m_Restart = false;
        Engine::Registry().clear();

        racing_game::SetupWorld(Engine::Instance());
    }
}

void racing_game::RacingToolsSystem::RenderToolMenu()
{
    if (ImGui::BeginMenu("Race")) {

        if (ImGui::MenuItem("Restart"))
        {
            m_Restart = true;
        }

        ImGui::EndMenu();
    }
}

#endif