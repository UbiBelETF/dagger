#include "controls.h"
#include "mage.h"
#include "golem.h"
#include "towers.h"
#include "team_game_main.h"

#include "core/graphics/text.h"
#include "core/input/inputs.h"
#include "core/engine.h"

#include <unordered_map>

Bool ancient_defenders::PlayerControlsSystem::s_onStart = true;

using namespace ancient_defenders;

void ancient_defenders::PlayerControlsSystem::Run()
{
    if (PlayerControlsSystem::s_onStart == true) {
        Engine::Registry().view<InputReceiver>().each(
            [](InputReceiver& input_)
        {
            if (EPSILON_NOT_ZERO(input_.Get("restart")))
            {
                Engine::Registry().clear();


                ancient_defenders::SetupWorld(Engine::Instance());
                ancient_defenders::SetupControls(Engine::Instance());
                ancient_defenders::LoadPath();
                ancient_defenders::LoadTowerSpots();
                
                PlayerControlsSystem::s_onStart = false;
            }
        });
        return;
    }

    static std::unordered_map<String, SInt32> towerMenuChoiceOffsets{ 
        { "BLOOD", -120 },
        { "FIRE", -73 },
        { "ICE", -25 },
        { "POISON", 25 },
        { "STORM", 71 },
        { "SUN", 120 },
    };

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

    auto towerInfo = Engine::GetDefaultResource<TowerPlacementInfo>();

    for (UInt32 i = 0; i < towerInfo->spotCoordinates.size(); i++)
    {
        auto towerCoords = towerInfo->spotCoordinates[i];

        if (!towerInfo->availableSpot[i] && glm::distance(circle.position, { towerCoords, 0.0f }) < 1.0f)
        {
            circle.position = { 10000, 10000, 10000 };
            towerInfo->selectedSpot = TOWER_NONE;
        }

        if (towerInfo->availableSpot[i])
        {
            if (glm::distance(mouse, towerCoords) < 30)
            {
                towerInfo->selectedSpot = i;
                circle.position = { towerCoords, 0.0f };

                if (dagger::Input::IsInputDown(EDaggerMouse::MouseButton1))
                {
                    if (towerInfo->chantingMages[i] == 0) {
                        Tower::Create(towerInfo->selectedTower);
                    }

                    if (towerInfo->spotTowerNames[towerInfo->selectedSpot] == "")
                    {
                        towerInfo->spotTowerNames[towerInfo->selectedSpot] = towerInfo->selectedTower;
                    }

                    Mage::Create(towerInfo->spotCoordinates[towerInfo->selectedSpot], EAction::Chanting, true, towerInfo->selectedSpot);

                    Logger::info("Placed a tower");

                    dagger::Input::ConsumeInput(EDaggerMouse::MouseButton1);
                }

                if (!towerInfo->chantingMages[i])
                {
                    menu.position = { mouse - Vector2{0, 50}, 2.0f };
                    build.position = { 10000, 10000, 10000 };
                    buildIconTransform.position = { 10000, 10000, 10000 };
                    buildTimeText.Set("pixel-font", "", { 10000, 10000, 10000 });
                    if (menu.position.x < -250) menu.position.x = -250;
                    if (menu.position.x > 250) menu.position.x = 250;

                    if (towerInfo->selectedTower == "")
                        towerInfo->selectedTower = "BLOOD";

                    choice.position = menu.position + Vector3{ towerMenuChoiceOffsets[towerInfo->selectedTower], -20, -1 };
                }
                else
                {
                    menu.position = { 10000, 10000, 10000 };
                    choice.position = menu.position;
                    build.position = { mouse - Vector2{0, 50}, 3.0f };
                    if (build.position.x < -250) build.position.x = -250;
                    if (build.position.x > 250) build.position.x = 250;

                    auto towerName = towerInfo->spotTowerNames[towerInfo->selectedSpot];
                    if (towerName != "")
                    {
                        AssignSprite(buildIconSprite, fmt::format("ancient_defenders:{}", towerName));
                        buildIconTransform.position = build.position + Vector3{ -23, 5, -1 };
                        auto mageCount = towerInfo->chantingMages[towerInfo->selectedSpot];
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
        menu.position = choice.position = build.position = buildIconTransform.position = circle.position = { 10000, 10000, 10000 };
        buildTimeText.Set("pixel-font", "", { 10000, 10000, 10000 });

        towerInfo->selectedSpot = TOWER_NONE;
    }

    Engine::Registry().view<InputReceiver>().each(
        [towerInfo](InputReceiver& input_) 
    {        
        if (EPSILON_NOT_ZERO(input_.Get("move")))
        {
            Mage::Create(WalkingPath::path[0], EAction::Moving);
            Logger::info("Made mage");
        }

        if (EPSILON_NOT_ZERO(input_.Get("enemy")))
        {
			Golem::Create("LITTLE");
            Golem::Create("MIDDLE");
			Golem::Create("BIG");
        }

        if (EPSILON_NOT_ZERO(input_.Get("num")))
        {
            auto num = (UInt32) input_.Get("num");
            if (num <= 6)
            {
                towerInfo->selectedTower = towerInfo->towerNames[num - 1];
                Logger::info("Switched to tower: "+ towerInfo->selectedTower);
            }
        }

        if (EPSILON_NOT_ZERO(input_.Get("restart")))
        {
            Engine::Registry().clear();


            ancient_defenders::SetupWorld(Engine::Instance());
            ancient_defenders::SetupControls(Engine::Instance());
            ancient_defenders::LoadPath();
            ancient_defenders::LoadTowerSpots();
        
            Engine::ToggleSystemsPause(false); // If the game is restarted after the ending
        }

    });
}
