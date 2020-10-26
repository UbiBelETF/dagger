
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
#include "gameplay/jiggle.h"
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

void PostInitWorld(Engine &engine)
{
    ShaderSystem::Use("standard");

    auto& reg = engine.GetRegistry();

    // field
    constexpr int ScreenHeigh = 600;
    constexpr int ScreenWidth = 800;

    constexpr int Heigh = 20;
    constexpr int Width = 26;
    constexpr float TileSize = 30.f;// / static_cast<float>(Width);

    constexpr float Space = 0.02f;
    for (int i = 0; i < Heigh; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSpriteTexture(sprite, "EmptyWhitePixel");
            sprite.scale = TileSize;

            
            if (i%2 != j%2)
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

                auto& col = reg.emplace<SimpleCollision>(entity);
                col.size.x = TileSize;
                col.size.y = TileSize;
            }

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + j + j * Space - static_cast<float>(Width) / 2.f) * TileSize;
            transform.position.y = (0.5f + i + i * Space - static_cast<float>(Heigh) / 2.f) * TileSize;
        }
    }

    // ball
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "PingPong:ball");
        sprite.scale = TileSize;

        sprite.color.r = 0.1f;
        sprite.color.g = 0.8f;
        sprite.color.b = 0.0f;

        auto& transform = reg.emplace<Transform>(entity);
        auto& ball = reg.emplace<PingPongBall>(entity);
        ball.speed.x = 7 * TileSize;
        ball.speed.y = 7 * TileSize;

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = TileSize;
        col.size.y = TileSize;
    }
}

void PostInitWorld2(Engine &engine)
{
    ShaderSystem::Use("standard");

    auto& reg = engine.GetRegistry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    sprite.scale = 500.f;
    auto& anim = reg.emplace<Animator>(entity);
    AnimatorPlay(anim, "souls_like_knight_character:DRINK_POTION");
}

int main(int argc_, char** argv_)
{
	srand(time(0));
	Logger::set_level(Logger::level::trace);

	Engine engine;

    CoreSystemsSetup(engine);

    GameplaySystemsSetup(engine);

	EngineInit(engine);
	
	// Post-init pre-loop setup

    PostInitWorld(engine);
    //PostInitWorld2(engine);

	// Game loop starts here 

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
