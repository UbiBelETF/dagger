#include "team_game_main.h"
#include "mage.h"
#include "golem.h"
#include "range_of_attack.h"
#include "towers.h"
#include "hp_system.h"
#include "controls.h"

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
      engine_.AddSystem<SimpleCollisionsSystem>();
      engine_.AddSystem<RangedTargetingSystem>();
      engine_.AddSystem<MageBehaviorSystem>();
      engine_.AddSystem<TowerBehaviorSystem>();
      engine_.AddSystem<EnemyBehaviorSystem>();
      engine_.AddSystem<HealthManagementSystem>();
      engine_.AddSystem<PlayerControlsSystem>();
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

    ancient_defenders::SetupWorld(engine_);
    ancient_defenders::LoadPath();
    ancient_defenders::LoadTowerSpots();
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
        AssignSprite(sprite, "ancient_defenders:icons:spells");

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, 0, 0 };

        Engine::GetDefaultResource<TowerMenuState>()->towerMenuEntity = entity;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:icons:build");

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 10000, 10000, 10000 };

        Engine::GetDefaultResource<TowerMenuState>()->buildEntity = entity;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:BLOOD");

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 10000, 10000, 10000 };

        Engine::GetDefaultResource<TowerMenuState>()->buildImageEntity = entity;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:icons:pointer");

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 10000, 10000, 10000 };

        Engine::GetDefaultResource<TowerMenuState>()->towerMenuChoiceEntity = entity;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        auto& text = reg.emplace<Text>(entity);

        text.alignment = TextAlignment::CENTER;
        text.Set("pixel-font", "1");
        Engine::GetDefaultResource<TowerMenuState>()->buildTimeTextEntity = entity;
    }

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "ancient_defenders:selector");

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

    FileInputStream inFile{ "spots.txt" };

    Vector2 point;

    while (inFile >> point.x >> point.y) {
        TowerPlacementInfo::spotCoordinates.emplace_back(point);
    }
}