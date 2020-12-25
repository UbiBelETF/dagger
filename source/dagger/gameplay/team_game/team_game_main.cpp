#include "team_game_main.h"
#include "mage.h"
#include "golem.h"
#include "range_of_attack.h"
#include "towers.h"
#include "hp_system.h"
#include "controls.h"
#include "spell.h"
#include "game_manager.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/graphics/text.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"


using namespace dagger;
using namespace ancient_defenders;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
      engine_.AddPausableSystem<SimpleCollisionsSystem>();
      engine_.AddPausableSystem<RangedTargetingSystem>();
      engine_.AddPausableSystem<SpellBehaviorSystem>();
      engine_.AddPausableSystem<MageBehaviorSystem>();
      engine_.AddPausableSystem<EnemyBehaviorSystem>();
      engine_.AddPausableSystem<TowerBehaviorSystem>();
      engine_.AddPausableSystem<HealthManagementSystem>();
      engine_.AddPausableSystem<PlayerControlsSystem>();
      engine_.AddPausableSystem<GameManagerSystem>();
}

void TeamGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    ancient_defenders::SetupStartScreen(engine_);
    ancient_defenders::SetupControls(engine_);
}

void ancient_defenders::SetupWorld(Engine &engine_)
{
    auto& reg = engine_.Registry();

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:level1-ground");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 800 , 600  };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, 0, 100 };
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);

        AssignSprite(sprite, "spritesheets:hp-bar:hp_BCK");
        sprite.scale = { 10,5 };
        sprite.position = { 0, -260, 99 };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, -260, 99 };
    }
    {

        auto entity = Engine::Registry().create();
        auto& player = Engine::Registry().emplace<PlayerInfo>(entity);

        player.hpSprite = reg.create();

        auto& sprite = reg.emplace<Sprite>(player.hpSprite);

        AssignSprite(sprite, "spritesheets:hp-bar:hp_100");
        sprite.scale = { 10,5 };
        sprite.position = { 0, -260, 98 };

        auto& transform = reg.emplace<Transform>(player.hpSprite);
        transform.position = { 0, -260, 98 };

        player.countdownTimer = reg.create();
        auto& sprite2 = reg.emplace<Sprite>(player.countdownTimer);
        
        auto& text = reg.emplace<Text>(player.countdownTimer);

        text.alignment = TextAlignment::CENTER;
        text.Set("pixel-font", fmt::format("Raid: {}s left!", (UInt32)player.timeLeft), { 10, 260, 98 });

        Engine::PutDefaultResource<PlayerInfo>(&player);
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:icons:spells");

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, 0, 0 };

        Engine::GetDefaultResource<TowerMenuState>()->towerMenuEntity = entity;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:icons:build");
        sprite.position = { 10000, 10000, 10000 };
        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 10000, 10000, 10000 };

        Engine::GetDefaultResource<TowerMenuState>()->buildEntity = entity;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:BLOOD");
        sprite.position = { 10000, 10000, 10000 };
        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 10000, 10000, 10000 };

        Engine::GetDefaultResource<TowerMenuState>()->buildImageEntity = entity;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:icons:pointer");
        sprite.position = { 10000, 10000, 10000 };
        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 10000, 10000, 10000 };

        Engine::GetDefaultResource<TowerMenuState>()->towerMenuChoiceEntity = entity;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        auto& text = reg.emplace<Text>(entity);
        sprite.position = { 10000, 10000, 10000 };
        text.alignment = TextAlignment::CENTER;
        text.Set("pixel-font", "1");
        Engine::GetDefaultResource<TowerMenuState>()->buildTimeTextEntity = entity;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:selector");
        sprite.position = { 10000, 10000, 10000 };
        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 10000, 10000, 10000 };

        Engine::GetDefaultResource<TowerMenuState>()->towerSelectionEntity = entity;
    }
}




void ancient_defenders::SetupControls(Engine& engine_) {
    auto& reg = engine_.Registry();

    {
        auto entity = reg.create();
        auto & inputs = reg.emplace<InputReceiver>(entity);

        inputs.contexts.push_back("AD_Controls");
    }
}

void ancient_defenders::LoadPath() {

    FileInputStream inFile{ "path.txt" };

    Vector2 point;

    while (inFile >> point.x >> point.y) {
       WalkingPath::path.emplace_back(point);
    }

    WalkingPath::numberOfPoints = WalkingPath::path.size();
}

void ancient_defenders::LoadTowerSpots() {

    auto entity = Engine::Registry().create();
    auto& towers = Engine::Registry().emplace<TowerPlacementInfo>(entity);
    towers.selectedSpot = TOWER_NONE;
    towers.selectedTower = "BLOOD";

    towers.spotCoordinates = {};
    towers.availableSpot = { true, true, true, true, true, true, true, true };
    towers.chantingMages = { 0, 0, 0, 0, 0, 0, 0, 0 };
    towers.spotTowerNames = { "", "", "", "", "", "", "", "" };
    towers.towerNames = { "BLOOD", "FIRE", "ICE", "POISON", "STORM", "SUN" };

    

    FileInputStream inFile{ "spots.txt" };

    Vector2 point;

    while (inFile >> point.x >> point.y) {
        towers.spotCoordinates.emplace_back(point);
    }

    Engine::PutDefaultResource<TowerPlacementInfo>(&towers);
}

void ancient_defenders::SetupEndScreen(Engine & engine_, Bool goodEnd_)
{
    auto& reg = engine_.Registry();

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);

        auto& text = reg.emplace<Text>(entity);

        text.alignment = TextAlignment::CENTER;
        text.Set("pixel-font", "Ancient has been", { 0, 15, 1 });
        text.spacing = 0.6f;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);

        auto& text = reg.emplace<Text>(entity);

        text.alignment = TextAlignment::CENTER;
        text.Set("pixel-font", goodEnd_?"successfully defended":"destroyed", { 0, -15, 1 });
        text.spacing = 0.6f;
    }


    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);

        auto& text = reg.emplace<Text>(entity);

        text.alignment = TextAlignment::CENTER;
        text.Set("pixel-font", goodEnd_?"good work":"better luck next time", { 0, -100, 1 });
        text.spacing = 0.6f;
    }


    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);

        auto& text = reg.emplace<Text>(entity);

        text.alignment = TextAlignment::CENTER;
        text.Set("pixel-font", "Press Enter to restart", { 0, -200, 1 });
        text.spacing = 0.6f;
    }
}

void ancient_defenders::SetupStartScreen(Engine & engine_)
{
    auto& reg = engine_.Registry();

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:level1-ground");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 800 , 600 };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, 0, 100 };
    }

    {
        auto entity = reg.create();
        
        auto& text = reg.emplace<Text>(entity);

        text.alignment = TextAlignment::CENTER;
        text.Set("pixel-font", "Ancient Defenders", { 0, 200, 1 });
        text.spacing = 0.6;
    }

    {
        auto entity = reg.create();
        
        auto& text = reg.emplace<Text>(entity);

        text.alignment = TextAlignment::CENTER;
        text.Set("pixel-font", "Press Enter to start", { 0, -200, 1 });
        text.spacing = 0.6f;
    }
}