#include "plight_game_logic.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"

#include "gameplay/plight/plight.h"

using namespace dagger;
using namespace plight;

void PlightGameLogicSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&PlightGameLogicSystem::OnEndOfFrame>(this);
}

void PlightGameLogicSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&PlightGameLogicSystem::OnEndOfFrame>(this);
}

void PlightGameLogicSystem::Run()
{
    
        auto view = Engine::Registry().view<InputReceiver>();
        for (auto entity : view)
        {
            auto& input = view.get<InputReceiver>(entity);
            if (EPSILON_NOT_ZERO(input.Get("restart"))) {
                m_Restart = true;
            }
        }
    
}

void PlightGameLogicSystem::OnEndOfFrame()
{
    if (m_Restart)
    {
        m_Restart = false;
        Engine::Registry().clear();

        plight::SetupTilemaps();
        plight::SetupWorld_AimingSystem(Engine::Instance());
    }
}