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
#include "gameplay/ping_pong/player_scores.h"
#include "gameplay/ping_pong/pingpong_playerinput.h"

using namespace dagger;
using namespace pingPong;

void pingPong::CreatePingPongBall(entt::registry &reg_, float TileSize_, Color color_, Vector3 speed_, Vector3 pos_)
{
    auto entity = reg_.create();
    auto& sprite = reg_.emplace<Sprite>(entity);
    AssignSpriteTexture(sprite, "PingPong:ball");
    sprite.scale = Vector2(1, 1) * TileSize_;

    sprite.color = color_;

    auto& transform = reg_.emplace<Transform>(entity);
    transform.position = pos_ * TileSize_;
    auto& ball = reg_.emplace<PingPongBall>(entity);
    ball.speed = speed_ * TileSize_;

    auto& col = reg_.emplace<SimpleCollision>(entity);
    col.size.x = TileSize_;
    col.size.y = TileSize_;
}

void pingPong::SetupSystems(Engine &engine_)
{
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<PingPongBallSystem>();
    engine_.AddSystem<PingPongPlayerInputSystem>();
    engine_.AddSystem<PlayerScoresSystem>();
}

void pingPong::SetupWorld(Engine &engine_)
{
    Vector2 scale(1, 1);

    ShaderSystem::Use("standard");

    Camera camera;
    camera.mode = ECameraMode::FixedResolution;
    camera.size = { 800, 600 };
    camera.zoom = 0.17f;
    Engine::Dispatcher().trigger<Camera>(camera);

    auto& reg = engine_.GetRegistry();

    // field
    constexpr int ScreenHeigh = 600;
    constexpr int ScreenWidth = 800;

    constexpr int Heigh = 20;
    constexpr int Width = 26;
    constexpr float TileSize = 1.f;// / static_cast<float>(Width);

    float zPos = -1.f;

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
            transform.position.z = zPos;
        }
    }

    zPos += 0.5f;

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
            transform.position.z = zPos;
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
            transform.position.z = zPos;
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
            transform.position.z = zPos;

            auto& wall = reg.emplace<PingPongWall>(entity);
            wall.isLeft = true;
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
            transform.position.z = zPos;

            auto& wall = reg.emplace<PingPongWall>(entity);
            wall.isLeft = false;
        }
    }

    // ball
    CreatePingPongBall(reg, TileSize, Color(1, 1, 1, 1), { 7,-7,0 },        { -1,5,zPos });
    //CreatePingPongBall(reg, TileSize, Color(0.5f, 1, 1, 1), { -14,14,0 },   { 1,3,zPos });
    CreatePingPongBall(reg, TileSize, Color(1, 0.5f, 1, 1), { -6,4,0 },      { -1,1,zPos });
    //CreatePingPongBall(reg, TileSize, Color(1, 1, 0.5f, 1), {- 7,-7,0 },    { 1,-1,zPos });
    //CreatePingPongBall(reg, TileSize, Color(0.5f, 0.5f, 1, 1), { 20,14,0 }, { -1,-3,zPos });
    //CreatePingPongBall(reg, TileSize, Color(0.5f, 0.5f, 0.5f, 1), { -14,-20,0 }, { 1,-5,zPos });
    CreatePingPongBall(reg, TileSize, Color(0.5f, 1, 0.5f, 1), { 8,8,0 },   { -1,-7,zPos });

    // player controller setup
    const Float32 playerSize = TileSize * ((Heigh - 2) * (1 + Space) * 0.33f);
    PingPongPlayerInputSystem::SetupPlayerBoarders(playerSize, -playerSize);
    PingPongPlayerInputSystem::s_PlayerSpeed = TileSize * 14.f;
    //1st player
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = TileSize;
        col.size.y = playerSize;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = (2.5f - static_cast<float>(Width * (1 + Space)) / 2.f) * TileSize;
        transform.position.y = 0;
        transform.position.z = zPos;

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
        transform.position.z = zPos;

        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "EmptyWhitePixel");
        sprite.scale.x = TileSize;
        sprite.scale.y = playerSize;

        auto& controller = reg.emplace<ControllerMapping>(entity);
        PingPongPlayerInputSystem::SetupPlayerTwoInput(controller);
    }

    // add score system to count scores for left and right collisions
    PlayerScoresSystem::SetFieldSize(Width, Heigh, TileSize * (1 + Space));
}
