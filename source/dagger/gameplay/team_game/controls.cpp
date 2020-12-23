#include "controls.h"
#include "mage.h"
#include "golem.h"
#include "towers.h"

#include "core/graphics/text.h"
#include "core/input/inputs.h"
#include "core/engine.h"

#include <unordered_map>

using namespace ancient_defenders;

void ancient_defenders::PlayerControlsSystem::Run()
{
    static std::unordered_map<String, SInt32> towerMenuChoiceOffsets{ 
        { "BLOOD", -120 },
        { "FIRE", -73 },
        { "ICE", -25 },
        { "POISON", 25 },
        { "STORM", 71 },
        { "SUN", 120 },
    };

    if ((m_controlData.timeout -= Engine::DeltaTime()) > 0.0f) return;
    // Controls are getting a very small timeout every time something spawns, because due to the input value not reseting in time
    // every comand executed multiple times, spawining multiple objects, which wasn't the intended behavior

    auto mouse = dagger::Input::CursorPositionInWorld();

    auto& menu = Engine::Registry().get<Transform>(Engine::GetDefaultResource<TowerMenuState>()->towerMenuEntity);
    auto& choice = Engine::Registry().get<Transform>(Engine::GetDefaultResource<TowerMenuState>()->towerMenuChoiceEntity);

    auto& build = Engine::Registry().get<Transform>(Engine::GetDefaultResource<TowerMenuState>()->buildEntity);
    auto& buildIconTransform = Engine::Registry().get<Transform>(Engine::GetDefaultResource<TowerMenuState>()->buildImageEntity);
    auto& buildIconSprite = Engine::Registry().get<Sprite>(Engine::GetDefaultResource<TowerMenuState>()->buildImageEntity);
    auto& buildTimeText = Engine::Registry().get<Text>(Engine::GetDefaultResource<TowerMenuState>()->buildTimeTextEntity);
    auto& buildTimeSprite = Engine::Registry().get<Sprite>(Engine::GetDefaultResource<TowerMenuState>()->buildTimeTextEntity);

    auto& circle = Engine::Registry().get<Transform>(Engine::GetDefaultResource<TowerMenuState>()->towerSelectionEntity);

    bool found = false;

    for (UInt32 i = 0; i < TowerPlacementInfo::spotCoordinates.size(); i++)
    {
        auto towerCoords = TowerPlacementInfo::spotCoordinates[i];

        if (!TowerPlacementInfo::availableSpot[i] && glm::distance(circle.position, { towerCoords, 0.0f }) < 1.0f)
        {
            circle.position = { 10000, 10000, 10000 };
            TowerPlacementInfo::selectedSpot = TOWER_NONE;
        }

        if (TowerPlacementInfo::availableSpot[i])
        {
            if (glm::distance(mouse, towerCoords) < 30)
            {
                TowerPlacementInfo::selectedSpot = i;
                circle.position = { towerCoords, 0.0f };

                if (dagger::Input::IsInputDown(EDaggerMouse::MouseButton1))
                {
                    Tower::Create(TowerPlacementInfo::selectedTower);
                    if (TowerPlacementInfo::spotTowerNames[TowerPlacementInfo::selectedSpot] == "")
                    {
                        TowerPlacementInfo::spotTowerNames[TowerPlacementInfo::selectedSpot] = TowerPlacementInfo::selectedTower;
                    }

                    Mage::Create(TowerPlacementInfo::spotCoordinates[TowerPlacementInfo::selectedSpot], EAction::Chanting, true, TowerPlacementInfo::selectedSpot);

                    Logger::info("Placed a tower");

                    m_controlData.timeout = ControlData::maxTimeout;

                    dagger::Input::ConsumeInput(EDaggerMouse::MouseButton1);
                }

                if (!TowerPlacementInfo::chantingMages[i])
                {
                    menu.position = { mouse - Vector2{0, 50}, 2.0f };
                    build.position = { 10000, 10000, 10000 };
                    buildIconTransform.position = { 10000, 10000, 10000 };
                    buildTimeText.Set("pixel-font", "", { 10000, 10000, 10000 });
                    if (menu.position.x < -250) menu.position.x = -250;
                    if (menu.position.x > 250) menu.position.x = 250;

                    if (TowerPlacementInfo::selectedTower == "")
                        TowerPlacementInfo::selectedTower = "BLOOD";

                    choice.position = menu.position + Vector3{ towerMenuChoiceOffsets[TowerPlacementInfo::selectedTower], -20, -1 };
                }
                else
                {
                    menu.position = { 10000, 10000, 10000 };
                    choice.position = menu.position;
                    build.position = { mouse - Vector2{0, 50}, 3.0f };
                    if (build.position.x < -250) build.position.x = -250;
                    if (build.position.x > 250) build.position.x = 250;

                    auto towerName = TowerPlacementInfo::spotTowerNames[TowerPlacementInfo::selectedSpot];
                    if (towerName != "")
                    {
                        AssignSprite(buildIconSprite, fmt::format("ancient_defenders:{}", towerName));
                        buildIconTransform.position = build.position + Vector3{ -23, 5, -1 };
                        auto mageCount = TowerPlacementInfo::chantingMages[TowerPlacementInfo::selectedSpot];
                        buildTimeText.Set("pixel-font", fmt::format("{}", mageCount), build.position + Vector3{ 43, 0, -2 });
                    }
                    else
                    {
                        buildIconTransform.position = { 10000, 10000, 10000 };
                        buildTimeText.Set("pixel-font", "", { 10000, 10000, 10000 });
                    }
                }

                found = true;
                break;
            }
        }
    }

    if (!found)
    {
        menu.position = choice.position = build.position = buildIconTransform.position = { 10000, 10000, 10000 };
        buildTimeText.Set("pixel-font", "", { 10000, 10000, 10000 });

        TowerPlacementInfo::selectedSpot = TOWER_NONE;
    }

    Engine::Registry().view<InputReceiver>().each(
        [this](InputReceiver& input_) 
    {        
        if (EPSILON_NOT_ZERO(input_.Get("move")))
        {
            Mage::Create(WalkingPath::path[0], EAction::Moving);
            Logger::info("Made mage");

            m_controlData.timeout = ControlData::maxTimeout;
        }

        if (EPSILON_NOT_ZERO(input_.Get("enemy")))
        {
            Golem::Create();

            m_controlData.timeout = ControlData::maxTimeout;
        }

        if (EPSILON_NOT_ZERO(input_.Get("num")))
        {
            auto num = (UInt32) input_.Get("num");
            if (num <= 6)
            {
                TowerPlacementInfo::selectedTower = TowerPlacementInfo::towerNames[num - 1];
                Logger::info("Switched to tower: "+ TowerPlacementInfo::selectedTower);
            }
        }
    });
}
