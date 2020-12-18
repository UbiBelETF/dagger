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

#include "gameplay/common/simple_collisions.h"
#include "gameplay/tank_warfare/tank_controller.h"
#include "gameplay/tank_warfare/tank_collision.h"
#include "gameplay/tank_warfare/camera_center.h"
#include "gameplay/tank_warfare/rocket.h"
#include "gameplay/tank_warfare/collectibles.h"
#include "gameplay/tank_warfare/tile_map.h"

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
}

void TankWarfare::WorldSetup(Engine &engine_)
{
    tank_warfare::SetupCamera(engine_);
    tank_warfare::SetupTestWorld(engine_);
}

void tank_warfare::SetupCamera(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
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
        sprite.size = { 500 / ratio, 500  };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, 0, zPos };

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;
    }
}

void tank_warfare::SetupTestWorld(Engine& engine_)
{
    auto& reg = Engine::Registry();
	Tilemap::legend['#'] = &CreateDirt;
	Tilemap::legend['.'] = &CreateGrass;
	Tilemap::legend['-'] = &CreateBuilding;
	Tilemap::legend['$'] = &CreateBankBuilding;
	Tilemap::legend['B'] = &CreateBiggestBuilding;
	Tilemap::legend['S'] = &CreateShopBuilding;
	Tilemap::legend['@'] = &CreateSmallestBuilding;
	Tilemap::legend['m'] = &CreateMediumBuilding;
	Tilemap::legend['t'] = &CreateTree;
	Tilemap::legend['T'] = &CreateGroupTrees;
	Tilemap::legend['w'] = &CreateHorizontalWall;
	Tilemap::legend['W'] = &CreateVerticallWall;
	Tilemap::legend['s'] = &CreateStorage;
	Tilemap::legend['f'] = &CreateSideFence;
	Tilemap::legend['F'] = &CreateFrontFence;
	Tilemap::legend['c'] = &CreateCarBack;
	Tilemap::legend['~'] = nullptr;

	for (auto& entry : Files::recursive_directory_iterator("textures\\jovanovici\\maps"))
	{
		if (entry.path().extension() == ".png") {
			auto entityMap = reg.create();
			auto& sprite = reg.emplace<Sprite>(entityMap);
			sprite.position = { 0, 0, 5 };
			AssignSprite(sprite, "jovanovici:maps:map");
		}
	}
	for (auto& entry : Files::recursive_directory_iterator("textures\\jovanovici\\maps"))
	{
		if (entry.path().extension() == ".map") {
			Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ entry.path().string(), &Tilemap::legend });
		}
	}
	//tank1
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    auto& anim = reg.emplace<Animator>(entity);
    auto& transform = reg.emplace<Transform>(entity);
    auto& collision = reg.emplace<SimpleCollision>(entity);
    auto& input = reg.emplace<InputReceiver>(entity);
    auto& tank = reg.emplace<TankCharacter>(entity);
	auto& cam = reg.emplace<CameraCenter>(entity);
    sprite.scale = { -1, 1 };
    transform.position = { 160, 0, 1 };
    collision.size = sprite.size;
    input.contexts.push_back("tank1");
    AssignSprite(sprite, "jovanovici:tank:tank3_side");

    //tank2
    auto entity2 = reg.create();
    auto& sprite2 = reg.emplace<Sprite>(entity2);
    auto& anim2 = reg.emplace<Animator>(entity2);
    auto& transform2 = reg.emplace<Transform>(entity2);
    auto& collision2 = reg.emplace<SimpleCollision>(entity2);
    auto& input2 = reg.emplace<InputReceiver>(entity2);
    auto& tank2 = reg.emplace<TankCharacter>(entity2);
    auto& cam2 = reg.emplace<CameraCenter>(entity2);
    sprite2.scale = { -1, 1 };
    transform2.position = { -160, 0, 1 };
    collision2.size = sprite2.size;
    input2.contexts.push_back("tank2");
    AssignSprite(sprite2, "jovanovici:tank:tank3_side");

}
