#include "ping_pong_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

#include "gameplay/simple_collisions.h"
#include "gameplay/ping_pong/pingpong_ball.h"
#include "gameplay/ping_pong/pingpong_playerinput.h"

using namespace dagger;
using namespace pingPong;

void CreatePingPongBall(entt::registry &reg, float TileSize, Color color, Vector3 speed, Vector3 pos)
{
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSpriteTexture(sprite, "PingPong:ball");
    sprite.scale = Vector2(1, 1) * TileSize;

    sprite.color = color;

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = pos * TileSize;
    auto& ball = reg.emplace<PingPongBall>(entity);
    ball.speed = speed * TileSize;

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = TileSize;
    col.size.y = TileSize;
}

void pingPong::SetupSystems(Engine &engine)
{
    engine.AddSystem<SimpleCollisionsSystem>();
    engine.AddSystem<PingPongBallSystem>();
    engine.AddSystem<PingPongPlayerInputSystem>();
}

void pingPong::SetupWorld(Engine &engine)
{
    Vector2 scale(1, 1);

    ShaderSystem::Use("standard");

    Camera camera;
    camera.mode = CameraMode::FixedResolution;
    camera.size = { 800, 600 };
    camera.zoom = 0.15f;
    Engine::Dispatcher().trigger<Camera>(camera);

    auto& reg = engine.GetRegistry();

    // field
    constexpr int ScreenHeigh = 600;
    constexpr int ScreenWidth = 800;

    constexpr int Heigh = 20;
    constexpr int Width = 26;
    constexpr float TileSize = 1.f;// / static_cast<float>(Width);

    constexpr float Space = 0.1f;
    for (int i = 0; i < Heigh; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSpriteTexture(sprite, "EmptyWhitePixel");
            sprite.scale = scale * TileSize;

            if (i % 2 != j % 2)
            {
                sprite.color.r = 0.4f;
                sprite.color.g = 0.4f;
                sprite.color.b = 0.4f;
            }
            else
            {
                sprite.color.r = 0.6f;
                sprite.color.g = 0.6f;
                sprite.color.b = 0.6f;
            }

            if (i == 0 || i == Heigh - 1 || j == 0 || j == Width - 1)
            {
                sprite.color.r = 0.0f;
                sprite.color.g = 0.0f;
                sprite.color.b = 0.0f;

                //auto& col = reg.emplace<SimpleCollision>(entity);
                //col.size.x = TileSize;
                //col.size.y = TileSize;
            }

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + j + j * Space - static_cast<float>(Width * (1 + Space)) / 2.f) * TileSize;
            transform.position.y = (0.5f + i + i * Space - static_cast<float>(Heigh * (1 + Space)) / 2.f) * TileSize;
        }
    }

    // collisions
    {
        // up
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = TileSize * (Width - 2)* (1 + Space);
            col.size.y = TileSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 0;
            transform.position.y = (0.5f + (Heigh - 1) + (Heigh - 1) * Space - static_cast<float>(Heigh * (1 + Space)) / 2.f) * TileSize;
        }

        // down
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = TileSize * (Width - 2) * (1 + Space);
            col.size.y = TileSize;

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = 0;
            transform.position.y = (0.5f - static_cast<float>(Heigh * (1 + Space)) / 2.f) * TileSize;
        }

        // left
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = TileSize;
            col.size.y = TileSize * (Heigh - 2) * (1 + Space);

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f - static_cast<float>(Width * (1 + Space)) / 2.f) * TileSize;
            transform.position.y = 0;
        }

        // right
        {
            auto entity = reg.create();
            auto& col = reg.emplace<SimpleCollision>(entity);
            col.size.x = TileSize;
            col.size.y = TileSize * (Heigh - 2) * (1 + Space);

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + (Width - 1) + (Width - 1) * Space - static_cast<float>(Width * (1 + Space)) / 2.f) * TileSize;
            transform.position.y = 0;
        }
    }

    // ball
    CreatePingPongBall(reg, TileSize, Color(1, 1, 1, 1), { 7,7,0 }, { 0,0,0 });
    CreatePingPongBall(reg, TileSize, Color(0.5f, 1, 1, 1), { 14,14,0 }, { 5,0,0 });
    CreatePingPongBall(reg, TileSize, Color(1, 0.5f, 1, 1), { 4,7,0 }, { 0,5,0 });
    CreatePingPongBall(reg, TileSize, Color(1, 1, 0.5f, 1), { 7,4,0 }, { -5,-5,0 });
    CreatePingPongBall(reg, TileSize, Color(0.5f, 0.5f, 1, 1), { 24,14,0 }, { 3,0,0 });
    CreatePingPongBall(reg, TileSize, Color(0.5f, 0.5f, 0.5f, 1), { 14,24,0 }, { 5,3,0 });
    CreatePingPongBall(reg, TileSize, Color(0.5f, 1, 0.5f, 1), { 5,5,0 }, { 5,-7,0 });

    // player controller setup
    const Float32 playerSize = TileSize * ((Heigh - 2) * (1 + Space) * 0.33f);
    PingPongPlayerInputSystem::SetupPlayerBoarders(playerSize, -playerSize);
    PingPongPlayerInputSystem::PLAYER_SPEED = TileSize * 10.f;
    //1st player
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = TileSize;
        col.size.y = playerSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = (2.5f - static_cast<float>(Width * (1 + Space)) / 2.f) * TileSize;
        transform.position.y = 0;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "EmptyWhitePixel");
        sprite.scale.x = TileSize;
        sprite.scale.y = playerSize;

        auto& controller = reg.emplace<ControllerMapping>(entity);
        PingPongPlayerInputSystem::SetupPlayerOneInput(controller);
    }

    //2nd player
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = TileSize;
        col.size.y = playerSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = (0.5f + (Width - 3) + (Width - 1) * Space - static_cast<float>(Width * (1 + Space)) / 2.f) * TileSize;
        transform.position.y = 0;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "EmptyWhitePixel");
        sprite.scale.x = TileSize;
        sprite.scale.y = playerSize;

        auto& controller = reg.emplace<ControllerMapping>(entity);
        PingPongPlayerInputSystem::SetupPlayerTwoInput(controller);
    }

    // add score system to count scores for left and right collisions
}
