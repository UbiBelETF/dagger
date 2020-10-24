
#include "core/optimization.h"
#include "core/engine.h"
#include "core/graphics/textures.h"
#include "core/graphics/texture.h"
#include "core/graphics/window.h"
#include "core/graphics/sprite.h"
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
    engine.AddSystem<TransformSystem>();
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
    constexpr float UnitSize = 0.94f / static_cast<float>(Width);

    constexpr float Space = 0.f;
    for (int i = 0; i < Heigh; i++)
    {
        for (int j = 0; j < Width; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            sprite.UseTexture("EmptyWhitePixel");
            sprite.m_Scale = UnitSize;

            
            if (i%2 != j%2)
            {
                sprite.m_Color.r = 0.5f;
                sprite.m_Color.g = 0.5f;
                sprite.m_Color.b = 0.5f;
            }

            if (i == 0 || i == Heigh - 1 || j == 0 || j == Width - 1)
            {
                sprite.m_Color.r = 0.0f;
                sprite.m_Color.g = 0.0f;
                sprite.m_Color.b = 0.0f;
            }

            auto& transform = reg.emplace<Transform>(entity);
            transform.position.x = j + j * Space - Width / 2.f;
            transform.position.y = i + i * Space - Heigh / 2.f;
        }
    }

    // ball
    {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        sprite.UseTexture("EmptyWhitePixel");
        sprite.m_Scale = UnitSize;

        sprite.m_Color.r = 0.1f;
        sprite.m_Color.g = 0.8f;
        sprite.m_Color.b = 0.0f;

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
#ifndef NDEBUG
	engine.AddSystem<ToolMenuSystem>();
	engine.AddSystem<DiagnosticSystem>();
	engine.AddSystem<ConsoleSystem>();
	engine.AddSystem<GUISystem>();
#endif // !NDEBUG
    GameplaySystemsSetup(engine);


	EngineInit(engine);
	
	// Post-init pre-loop setup

    PostInitWorld(engine);

	// Game loop starts here 

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
