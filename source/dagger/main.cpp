
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

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

using namespace dagger;

int main(int argc_, char** argv_)
{
	srand(time(0));
	Logger::set_level(Logger::level::trace);

	Engine engine;

	engine.AddSystem<WindowSystem>(1200, 800);
	engine.AddSystem<ShaderSystem>();
	engine.AddSystem<TextureSystem>();
	engine.AddSystem<SpriteRenderSystem>();
#ifndef NDEBUG
	engine.AddSystem<ToolMenuSystem>();
	engine.AddSystem<DiagnosticSystem>();
	engine.AddSystem<ConsoleSystem>();
	engine.AddSystem<GUISystem>();
#endif // !NDEBUG
    engine.AddSystem<JiggleSystem>();

	EngineInit(engine);
	
	// Post-init pre-loop setup

	ShaderSystem::Use("standard");

	auto& reg = engine.GetRegistry();

	for (int i = 0; i < 3; i++)
	{
		auto entity = reg.create();
		auto& sprite = reg.emplace<Sprite>(entity);
		sprite.UseTexture("rayman");
		sprite.m_Scale = 0.5f;
	}

	// Game loop starts here 

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
