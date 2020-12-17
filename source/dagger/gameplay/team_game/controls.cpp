#include "controls.h"
#include "mage.h"
#include "golem.h"
#include "towers.h"

#include "core/input/inputs.h"
#include "core/engine.h"

using namespace ancient_defenders;

Float32 ancient_defenders::ControlData::timeout = 0.0f;
Float32 ancient_defenders::ControlData::maxTimeout = 0.2f;

void ancient_defenders::PlayerControlsSystem::Run()
{
    if ((ControlData::timeout -= Engine::DeltaTime()) > 0.0f) return;
    // Controls are getting a very small timeout every time something spawns, because due to the input value not reseting in time
    // every comand executed multiple times, spawining multiple objects, which wasn't the intended behavior


    Engine::Registry().view<InputReceiver>().each(
        [](InputReceiver& input_) 
    {

        if (EPSILON_NOT_ZERO(input_.Get("move")))
        {
            Mage::Create(WalkingPath::path[0], EAction::Moving);
            Logger::info("Made mage");

            ControlData::timeout = ControlData::maxTimeout;
        }
        if (EPSILON_NOT_ZERO(input_.Get("chant")))
        {
            Tower::Create(TowerPlacementInfo::selectedTower);
            
            Mage::Create(TowerPlacementInfo::spotCoordinates[TowerPlacementInfo::selectedSpot], EAction::Chanting);

            Logger::info("Placed a tower");

            ControlData::timeout = ControlData::maxTimeout;
        }
        if (EPSILON_NOT_ZERO(input_.Get("enemy")))
        {
            Golem::Create();

            ControlData::timeout = ControlData::maxTimeout;
        }
        if (EPSILON_NOT_ZERO(input_.Get("num")))
        {
            if (TowerPlacementInfo::selectMode == SelectMode::Spot) {
                if (TowerPlacementInfo::availableSpot[input_.Get("num") - 1]) {
                    TowerPlacementInfo::selectedSpot = input_.Get("num") - 1;
                    Logger::info("Switched to spot: " + std::to_string(TowerPlacementInfo::selectedSpot));
                }
            }
            else if (TowerPlacementInfo::selectMode == SelectMode::Tower && input_.Get("num") <= 6) {
                TowerPlacementInfo::selectedTower = TowerPlacementInfo::towerNames[input_.Get("num") - 1];
                Logger::info("Switched to tower: "+ TowerPlacementInfo::selectedTower);
            }
        }
        if (EPSILON_NOT_ZERO(input_.Get("spot")))
        {
            TowerPlacementInfo::selectMode = SelectMode::Spot;
            Logger::info("Switched to Spot selecting");
        }
        if (EPSILON_NOT_ZERO(input_.Get("tower")))
        {
            TowerPlacementInfo::selectMode = SelectMode::Tower;
            Logger::info("Switched to Tower selecting");
        }

        
    });
}
