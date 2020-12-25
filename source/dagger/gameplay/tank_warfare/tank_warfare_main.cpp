#include "tank_warfare_main.h"
#include <iostream>
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/text.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/tank_warfare/tank_controller.h"
#include "gameplay/tank_warfare/tank_collision.h"
#include "gameplay/tank_warfare/camera_center.h"
#include "gameplay/tank_warfare/rocket.h"
#include "gameplay/tank_warfare/collectibles.h"
#include "gameplay/tank_warfare/tile_map.h"
#include "gameplay/tank_warfare/game_menu.h"

using namespace dagger;
using namespace tank_warfare;

void TankWarfare::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<TilemapSystem>();
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<TankControllerSystem>();
    engine_.AddSystem<TankCollisionSystem>();
    engine_.AddSystem<CameraCenterSystem>();
    engine_.AddSystem<RocketSystem>();
    engine_.AddSystem<CollectibleSystem>();
    engine_.AddSystem<GameMenuSystem>();
    engine_.AddSystem<TankStatsSystem>();
}

void TankWarfare::WorldSetup(Engine &engine_)
{
    tank_warfare::SetupStartScreen(engine_);
    //tank_warfare::SetupTestWorld(engine_);
    //tank_warfare::SetupRestartScreen(engine_);
}

void tank_warfare::SetupCamera(Engine &engine_, UInt32 zoom_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = zoom_;
    camera->position = { 0, 0, 0 };
    camera->Update();

    auto& reg = engine_.Registry();
    auto entity = reg.create();
    auto& camParams = reg.emplace<CameraParams>(entity);
    camParams.camZoom = camera->zoom;
    camParams.camXY = camera->size;
}

void tank_warfare::SetupWorld(Engine &engine_)
{
	auto& reg = engine_.Registry();

	float zPos = 1.f;

	{
		auto entity = reg.create();
		auto& sprite = reg.emplace<Sprite>(entity);
		AssignSprite(sprite, "logos:dagger");
		float ratio = sprite.size.y / sprite.size.x;
		sprite.size = { 500 / ratio, 500 };

		auto& transform = reg.emplace<Transform>(entity);
		transform.position = { 0, 0, zPos };

		auto& col = reg.emplace<SimpleCollision>(entity);
		col.size = sprite.size;
	}
}

void tank_warfare::SetupTestWorld(Engine& engine_)
{
    SetupCamera(engine_, 2);
    CollectibleSystem::ResetNumCoinsPowers();

    auto& reg = Engine::Registry();
	Tilemap::legend['-'] = &CreateBuilding;
	Tilemap::legend['$'] = &CreateBankBuilding;
	Tilemap::legend['B'] = &CreateBiggestBuilding;
	Tilemap::legend['@'] = &CreateSmallestBuilding;
	Tilemap::legend['m'] = &CreateMediumBuilding;
	Tilemap::legend['T'] = &CreateGroupTrees;
	Tilemap::legend['s'] = &CreateStorage;
	Tilemap::legend['E'] = &EmptyCollision;
	Tilemap::legend['~'] = nullptr;

    auto entityMap = reg.create();
    auto& sprite = reg.emplace<Sprite>(entityMap);
    AssignSprite(sprite, "jovanovici:maps:map");
    auto& transform = reg.emplace<Transform>(entityMap);
    transform.position = { 0, 0, 5 };

	for (auto& entry : Files::recursive_directory_iterator("maps"))
	{

	    if (entry.path().extension() == ".map") 
        {
	        Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ entry.path().string(), &Tilemap::legend });
	    }
	}
    
    //tank1
    CreateTankCharacter(1, { -150, 0, 3 }, "tank1");
    CreateUIBars({-250, 250, 0}, 1);

    //tank2
    CreateTankCharacter(2, { 150, 0, 3 }, "tank2");
    CreateUIBars({250, 250, 0}, 2);
}

void tank_warfare::SetupStartScreen(Engine& engine_)
{
    SetupCamera(engine_);
    auto& reg = engine_.Registry();
    
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    sprite.position.z = 2;
    AssignSprite(sprite, "jovanovici:menu");

    auto entity1 = reg.create();
    auto& sprite1 = reg.emplace<Sprite>(entity1);
    auto& anim1 = reg.emplace<Animator>(entity1);
    sprite1.position.z = 1;
    sprite1.position.y = 125;
    AssignSprite(sprite1, "jovanovici:logoJov:logo1");
    sprite1.size *= 3;
    AnimatorPlay(anim1, "logo:tankwarfare", 3);

    auto entity2 = reg.create();
    auto& sprite2 = reg.emplace<Sprite>(entity2);
    auto& gmb = reg.emplace<GameMenuButton>(entity2);
    auto& input = reg.emplace<InputReceiver>(entity2);
    input.contexts.push_back("menu");
    sprite2.position.z = 1;
    sprite2.position.y = -100;
    gmb.position = sprite2.position;
    AssignSprite(sprite2, "jovanovici:buttons:start1");
    sprite2.size *= 1.5;
    gmb.size = sprite2.size;
}

void tank_warfare::SetupRestartScreen(Engine& engine_, String winner_)
{
    SetupCamera(engine_);
    auto& reg = engine_.Registry();


    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    sprite.position.z = 2;
    AssignSprite(sprite, "jovanovici:menu");

    auto entityt1 = reg.create();
    auto& text1 = reg.emplace<Text>(entityt1);
    text1.Set("pixel-font", "player", {44, 175, 0}, 2);

    auto entityt2 = reg.create();
    auto& text2 = reg.emplace<Text>(entityt2);
    text2.Set("pixel-font", winner_, { 44, 100, 0 }, 2);

    auto entityt3 = reg.create();
    auto& text3 = reg.emplace<Text>(entityt3);
    text3.Set("pixel-font", "won", { 44, 25, 0 }, 2);

    auto entityc1 = reg.create();
    auto& spritec1 = reg.emplace<Sprite>(entityc1);
    auto& animc1 = reg.emplace<Animator>(entityc1);
    AssignSprite(spritec1, "jovanovici:confetti:confetti_01");
    spritec1.position.x = -250;
    spritec1.position.y = -200;
    AnimatorPlay(animc1, "confetti:confetti");

    auto entityc2 = reg.create();
    auto& spritec2 = reg.emplace<Sprite>(entityc2);
    auto& animc2 = reg.emplace<Animator>(entityc2);
    AssignSprite(spritec2, "jovanovici:confetti:confetti_01");
    spritec2.position.x = 250;
    spritec2.position.y = -200;
    AnimatorPlay(animc2, "confetti:confetti");

    auto entity2 = reg.create();
    auto& sprite2 = reg.emplace<Sprite>(entity2);
    auto& gmb = reg.emplace<GameMenuButton>(entity2);
    auto& input = reg.emplace<InputReceiver>(entity2);
    input.contexts.push_back("menu");
    sprite2.position.z = 1;
    sprite2.position.y = -100;
    gmb.position = sprite2.position;
    gmb.type = EGameMenuType::RestartScreen;
    AssignSprite(sprite2, "jovanovici:buttons:restart1");
    sprite2.size *= 1.5;
    gmb.size = sprite2.size;
}
