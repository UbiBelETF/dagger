#include "controls.h"
#include "mage.h"
#include "golem.h"
#include "towers.h"

#include "core/input/inputs.h"
#include "core/engine.h"

using namespace ancient_defenders;

void ancient_defenders::PlayerControlsSystem::Run()
{
    if ((m_controlData.timeout -= Engine::DeltaTime()) > 0.0f) return;
    // Controls are getting a very small timeout every time something spawns, because due to the input value not reseting in time
    // every comand executed multiple times, spawining multiple objects, which wasn't the intended behavior


    Engine::Registry().view<InputReceiver>().each(
        [this](InputReceiver& input_) 
    {

        if (EPSILON_NOT_ZERO(input_.Get("move")))
        {
            Mage::Create(WalkingPath::path[0], EAction::Moving);
            Logger::info("Made mage");

            m_controlData.timeout = ControlData::maxTimeout;
        }
        if (EPSILON_NOT_ZERO(input_.Get("chant")) && TowerPlacementInfo::availableSpot[TowerPlacementInfo::selectedSpot])
        {
            Tower::Create(TowerPlacementInfo::selectedTower);
            
            Mage::Create(TowerPlacementInfo::spotCoordinates[TowerPlacementInfo::selectedSpot], EAction::Chanting);

            Logger::info("Placed a tower");

            m_controlData.timeout = ControlData::maxTimeout;
        }
        if (EPSILON_NOT_ZERO(input_.Get("enemy")))
        {
            Golem::Create();

            m_controlData.timeout = ControlData::maxTimeout;
        }
        if (EPSILON_NOT_ZERO(input_.Get("num")))
        {
            if (TowerPlacementInfo::selectMode == ESelectMode::Spot) {
                if (TowerPlacementInfo::availableSpot[input_.Get("num") - 1]) {
                    TowerPlacementInfo::selectedSpot = input_.Get("num") - 1;
                    Logger::info("Switched to spot: " + std::to_string(TowerPlacementInfo::selectedSpot));
                }
            }
            else if (TowerPlacementInfo::selectMode == ESelectMode::Tower && input_.Get("num") <= 6) {
                TowerPlacementInfo::selectedTower = TowerPlacementInfo::towerNames[input_.Get("num") - 1];
                Logger::info("Switched to tower: "+ TowerPlacementInfo::selectedTower);
            }
        }
        if (EPSILON_NOT_ZERO(input_.Get("spot")))
        {
            TowerPlacementInfo::selectMode = ESelectMode::Spot;
            Logger::info("Switched to Spot selecting");
        }
        if (EPSILON_NOT_ZERO(input_.Get("tower")))
        {
            TowerPlacementInfo::selectMode = ESelectMode::Tower;
            Logger::info("Switched to Tower selecting");
        }

        
    });
}
