#include "controls.h"
#include "mage.h"
#include "golem.h"
#include "towers.h"

#include "core/engine.h"

using namespace ancient_defenders;

void ancient_defenders::PlayerControlsSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PlayerControlsSystem::OnKeyboardEvent>(this);
    Engine::Dispatcher().sink<NextFrame>().connect<&PlayerControlsSystem::OnEndOfFrame>(this);
}

void ancient_defenders::PlayerControlsSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PlayerControlsSystem::OnKeyboardEvent>(this);
    Engine::Dispatcher().sink<NextFrame>().disconnect<&PlayerControlsSystem::OnEndOfFrame>(this);
}

void ancient_defenders::PlayerControlsSystem::Run()
{
}

void ancient_defenders::PlayerControlsSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
    if (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held) {
        m_pressedKeys.emplace_back(kEvent_.key);
    }
    else if (kEvent_.action == EDaggerInputState::Released) {
    }
}

void ancient_defenders::PlayerControlsSystem::OnEndOfFrame()
{
    for (auto key : m_pressedKeys) {
        if (key == EDaggerKeyboard::KeyM)
        {
            Logger::info("Pressed M");
            Mage::Create(WalkingPath::path[0], EAction::Moving);
            Logger::info("Made mage");
        }
        else if (key == EDaggerKeyboard::KeyC)
        {
            if (TowerPlacementInfo::spotLocked == false) {
                TowerPlacementInfo::spotLocked = true;
                Tower::Create("STORM");
            }
            Mage::Create(TowerPlacementInfo::spotCoordinates[TowerPlacementInfo::selectedSpot], EAction::Chanting);
        }
        else if (key == EDaggerKeyboard::KeyS)
        {
            Golem::Create();
        }
        else if (key == EDaggerKeyboard::Key1)
        {
        }
    }

    m_pressedKeys.clear();
}
