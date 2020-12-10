#include "team_game_main.h"
#include "mage.h"
#include "golem.h"
#include "range_of_attack.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"


using namespace dagger;
using namespace ancient_defenders;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
      engine_.AddSystem<SimpleCollisionsSystem>();
      engine_.AddSystem<RangedTargetingSystem>();
      engine_.AddSystem<MageBehaviorSystem>();
	  engine_.AddSystem<GolemBehaviorSystem>();
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
    ancient_defenders::SetupDemoCharacter(engine_);
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
}



void ancient_defenders::SetupDemoCharacter(Engine& engine_) {
    auto& reg = engine_.Registry();

    float zPos = 1.f;


  	auto demoGolem = Golem::Create();
    auto demoMage = Mage::Create();
	/*

    auto demoMage1 = Mage::Create();
    auto demoMage2 = Mage::Create();
    auto demoMage3 = Mage::Create();

    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);

        AssignSprite(sprite, "spritesheets:mage:mage_stand_side:1");
        sprite.scale = { -2,2 };
        sprite.color = { 0.5f,0.5f,0.5f,0.5f };

        auto & coordinates = reg.emplace<Transform>(entity);
        coordinates.position = { 38, -178 , 1.0f };

        auto & en = reg.emplace<Enemy>(entity);
        en.health = 100.0f;

        auto & sc = reg.emplace<SimpleCollision>(entity);
        sc.size = sprite.size;
        auto & roa = reg.emplace<RangeOfAttack>(entity);
        roa.range = sc.size.x;

        roa.unitType = ETarget::Golem;
        roa.targetType = ETarget::Mage;
    }
	*/
	/*
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);

        AssignSprite(sprite, "spritesheets:mage:mage_stand_side:1");
        sprite.scale = { 2,2 };
        sprite.color = { 0.5f,0.5f,0.5f,0.5f };

        auto & coordinates = reg.emplace<Transform>(entity);
        coordinates.position = { 292, 8 , 1.0f };

        auto & en = reg.emplace<Enemy>(entity);
        en.health = 30.0f;

        auto & sc = reg.emplace<SimpleCollision>(entity);
        sc.size = sprite.size;
        auto & roa = reg.emplace<RangeOfAttack>(entity);
        roa.range = sc.size.x;

        roa.unitType = ETarget::Golem;
        roa.targetType = ETarget::Mage;
    }
	*/
	/*
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);

        AssignSprite(sprite, "spritesheets:mage:mage_stand_front:1");
        sprite.scale = { 2,2 };
        sprite.color = { 0.5f,0.5f,0.5f,0.5f };

        auto & coordinates = reg.emplace<Transform>(entity);
        coordinates.position = { -312, -100 , 1.0f };

        auto & en = reg.emplace<Enemy>(entity);
        en.health = 10.0f;

        auto & sc = reg.emplace<SimpleCollision>(entity);
        sc.size = sprite.size;
        auto & roa = reg.emplace<RangeOfAttack>(entity);
        roa.range = sc.size.x;

        roa.unitType = ETarget::Golem;
        roa.targetType = ETarget::Mage;
    }*/
}

void ancient_defenders::LoadPath() {

    FileInputStream inFile{ "path.txt" };

    Vector2 point;

    while (inFile >> point.x >> point.y) {
       WalkingPath::path.emplace_back(point);
    }

    WalkingPath::numberOfPoints = WalkingPath::path.size();
}