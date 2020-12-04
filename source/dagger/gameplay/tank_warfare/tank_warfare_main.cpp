#include "tank_warfare_main.h"

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

using namespace dagger;
using namespace tank_warfare;

void TankWarfare::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<TankControllerSystem>();
    engine_.AddSystem<TankCollisionSystem>();
}

void TankWarfare::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 0, 0, 0 };
    camera->Update();

    tank_warfare::SetupTestWorld(engine_);
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
    for (int i = -10; i < 10; i++)
    {
        for (int j = -10; j < 10; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, fmt::format("jovanovici:tile_map:tile_grass{}", 1 + (rand() % 3)));
            sprite.position = { i * 48, j * 48, 5 };
        }
    }

    //building
    auto entity1 = reg.create();
    auto& sprite1 = reg.emplace<Sprite>(entity1);
    auto& transform1 = reg.emplace<Transform>(entity1);
    auto& col1 = reg.emplace<SimpleCollision>(entity1);
    AssignSprite(sprite1, "jovanovici:building:building3c");
    transform1.position = { 100, 0, 2 };
    col1.size = sprite1.size;
    col1.size.x -= 10;
    col1.size.y -= 20;
    
    //tank1
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    auto& anim = reg.emplace<Animator>(entity);
    auto& transform = reg.emplace<Transform>(entity);
    auto& collision = reg.emplace<SimpleCollision>(entity);
    auto& input = reg.emplace<InputReceiver>(entity);
    auto& tank = reg.emplace<TankCharacter>(entity);
    sprite.scale = { -1, 1 };
    transform.position = { 35, 0, 1 };
    collision.size = sprite.size;
    input.contexts.push_back("tank1");
    AssignSprite(sprite, "jovanovici:tank:tank3_side");

}
