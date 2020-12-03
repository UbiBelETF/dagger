#include "team_game_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "tools/diagnostics.h"

#include "gameplay/team_game/team_game_collisions.h"
#include "gameplay/team_game/team_game_controller.h"

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<PlatformerControllerSystem>();
    engine_.AddSystem<PlatformerCollisionSystem>();
}

void TeamGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 0, 0, 0 };
    camera->Update();

    team_game::SetupWorld(engine_);
}

void team_game::SetupWorld(Engine &engine_)
{
    auto& reg = engine_.Registry();

    float zPos = 1.f;

    //Black Backdrop
    {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "EmptyWhitePixel");
        sprite.color = { 0, 0, 0, 1 };
        sprite.size = { 1000, 1000 };
        sprite.scale = { 10, 1 };
        sprite.position = { 0, -125, 10 };
    }
    
    {//First Character
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "EmptyWhitePixel");
        sprite.size = { 20, 40 };
        sprite.position = { -100, 40, 5 };

        auto& transform = reg.get_or_emplace<Transform>(entity);
        transform.position = sprite.position;

        auto& collision = reg.get_or_emplace<PlatformerCollision>(entity);
        collision.size = { 20, 40 };
        collision.entityType = PlatformerCollisionID::PLAYER;
        collision.state = MovementState::MOVEABLE;

        auto& controller = reg.get_or_emplace<PlatformerCharacter>(entity);
        controller.speed = 50;

        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        String inputContext = "ASDW";
        input.contexts.push_back(inputContext);
    }

    {//Second Character
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "EmptyWhitePixel");
        sprite.size = { 20, 40 };
        sprite.position = { 100, 40, 5 };

        auto& transform = reg.get_or_emplace<Transform>(entity);
        transform.position = sprite.position;

        auto& collision = reg.get_or_emplace<PlatformerCollision>(entity);
        collision.size = { 20, 40 };
        collision.entityType = PlatformerCollisionID::PLAYER;
        collision.state = MovementState::MOVEABLE;

        auto& controller = reg.get_or_emplace<PlatformerCharacter>(entity);
        controller.speed = 50;

        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        String inputContext = "Arrows";
        input.contexts.push_back(inputContext);
    }

    {//Platform they stand on
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "EmptyWhitePixel");
        sprite.size = { 1000, 20 };
        sprite.position = { 0, 0, 5 };

        auto& transform = reg.get_or_emplace<Transform>(entity);
        transform.position = sprite.position;

        auto& collision = reg.get_or_emplace<PlatformerCollision>(entity);
        collision.size = { 1000, 20 };
        collision.entityType = PlatformerCollisionID::TERRAIN;
        collision.state = MovementState::IMMOBILE;

        auto& controller = reg.get_or_emplace<PlatformerCharacter>(entity);
    }
}
