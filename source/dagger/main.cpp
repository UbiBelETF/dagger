
#include "core/optimization.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/textures.h"
#include "core/graphics/texture.h"
#include "core/graphics/window.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/animations.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/gui.h"
#include "tools/diagnostics.h"
#include "tools/console.h"
#include "tools/toolmenu.h"

#include "gameplay/examples/character_controller.h"

#include "gameplay/transform.h"
#include "gameplay/pingPongBall.h"
#include "gameplay/SimpleCollisions.h"

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

using namespace dagger;

void CoreSystemsSetup(Engine &engine)
{
    engine.AddSystem<WindowSystem>(800, 600);
    engine.AddSystem<InputSystem>();
    engine.AddSystem<ShaderSystem>();
    engine.AddSystem<TextureSystem>();
    engine.AddSystem<SpriteRenderSystem>();
    engine.AddSystem<AnimationSystem>();
    engine.AddSystem<TransformSystem>();
#if !defined(NDEBUG)
    engine.AddSystem<DiagnosticSystem>();
    engine.AddSystem<GUISystem>();
    engine.AddSystem<ToolMenuSystem>();
#endif //!defined(NDEBUG)
}

void GameplaySystemsSetup(Engine &engine)
{
    engine.AddSystem<SimpleCollisionsSystem>();
    engine.AddSystem<PingPongBallSystem>();
}

void CreatePingPongBall(entt::registry &reg, float TileSize, Color color, Vector3 speed, Vector3 pos)
{
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    AssignSpriteTexture(sprite, "PingPong:ball");
    sprite.scale = Vector2(1,1) * TileSize;

    sprite.color = color;

    auto& transform = reg.emplace<Transform>(entity);
    transform.position = pos * TileSize;
    auto& ball = reg.emplace<PingPongBall>(entity);
    ball.speed = speed * TileSize;

    auto& col = reg.emplace<SimpleCollision>(entity);
    col.size.x = TileSize;
    col.size.y = TileSize;

    auto& kb = reg.emplace<ControllerMapping>(entity);
    kb.key = DaggerKeyboard::KeyW;
}

void WorldSetup(Engine &engine)
{
    Vector2 scale(1, 1);

    ShaderSystem::Use("standard");

    Camera camera;
    camera.mode = CameraMode::FixedResolution;
    camera.size = { 800, 600 };
    Engine::Dispatcher().trigger<Camera>(camera);

    auto& reg = engine.GetRegistry();

    // field
    constexpr int ScreenHeigh = 600;
    constexpr int ScreenWidth = 800;

    constexpr int Heigh = 20;
    constexpr int Width = 26;
    constexpr float TileSize = 0.15f;// / static_cast<float>(Width);

    constexpr float Space = 0.1f;
    for (int i = 0; i < Heigh; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSpriteTexture(sprite, "EmptyWhitePixel");
            sprite.scale =  scale* TileSize;

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
    //         auto& kb = reg.emplace<ControllerMapping>(entity);
    //         kb.key = GLFW_KEY_UP;

        // player controller setup

        // add score system to count scores for left and right collisions
}

//void GameplaySystemsSetup(Engine& engine)
//{
//    example1::SetupSystems(engine);
//}
//
//void WorldSetup(Engine& engine)
//{
//    example1::SetupWorld(engine);
//}

int main(int argc_, char** argv_)
{
	Engine engine;
    CoreSystemsSetup(engine);
    GameplaySystemsSetup(engine);

    engine.EngineInit();
    WorldSetup(engine);

	while (engine.Up())
		engine.EngineLoop();

    engine.EngineStop();

	return 0;
}
