
#include "core/optimization.h"
#include "core/engine.h"
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

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

using namespace dagger;


void GameplaySystemsSetup(Engine &engine)
{
    //engine.AddSystem<JiggleSystem>();
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
    constexpr float UnitSize = 30.f;// / static_cast<float>(Width);

    constexpr float Space = 0.00f;
    for (int i = 0; i < Heigh; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSpriteTexture(sprite, "EmptyWhitePixel");
            sprite.scale = UnitSize;

            
            if (i%2 != j%2)
            {
                sprite.color.r = 0.5f;
                sprite.color.g = 0.5f;
                sprite.color.b = 0.5f;
            }

            if (i == 0 || i == Heigh - 1 || j == 0 || j == Width - 1)
            {
                sprite.color.r = 0.0f;
                sprite.color.g = 0.0f;
                sprite.color.b = 0.0f;
            }

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = (0.5f + j + j * Space - static_cast<float>(Width) / 2.f) * UnitSize;
            transform.position.y = (0.5f + i + i * Space - static_cast<float>(Heigh) / 2.f) * UnitSize;
        }
    }

    // ball
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSpriteTexture(sprite, "EmptyWhitePixel");
        sprite.scale = UnitSize;

        sprite.color.r = 0.1f;
        sprite.color.g = 0.8f;
        sprite.color.b = 0.0f;

        auto& transform = reg.emplace<Transform>(entity);
        auto& ball = reg.emplace<PingPongBall>(entity);
        ball.speed.x = 0.02f;
        ball.speed.y = 0.015f;
    }
}

int main(int argc_, char** argv_)
{
	srand(time(0));
	Logger::set_level(Logger::level::trace);

	Engine engine;

	engine.AddSystem<WindowSystem>(800, 600);
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

    GameplaySystemsSetup(engine);

	EngineInit(engine);
	
	// Post-init pre-loop setup

    PostInitWorld(engine);
    /*ShaderSystem::Use("standard");

    auto& reg = engine.GetRegistry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    auto& anim = reg.emplace<Animator>(entity);
    AnimatorPlay(anim, "souls_like_knight_character:DRINK_POTION");*/

	// Game loop starts here 

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
