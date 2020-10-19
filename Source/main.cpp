
#include "optimization.h"
#include "engine.h"
#include "textures.h"
#include "texture.h"
#include "window.h"
#include "diag.h"
#include "gui.h"
#include "console.h"
#include "sprite.h"
#include "sprite_render.h"
#include "toolmenu.h"
#include "jiggle.h"

#include <spdlog/spdlog.h>

#include <glm.hpp>

using namespace dagger;

int main(int argc_, char** argv_)
{
	srand(time(NULL));
	spdlog::set_level(spdlog::level::trace);

	Engine engine;

	engine.AddSystem<WindowSystem>(800, 800);
	engine.AddSystem<ShaderSystem>();
	engine.AddSystem<TextureSystem>();
	engine.AddSystem<SpriteRenderSystem>();
	//engine.AddSystem<ResourceManager>();
#ifndef NDEBUG
	engine.AddSystem<ToolMenuSystem>();
	engine.AddSystem<DiagnosticSystem>();
	engine.AddSystem<ConsoleSystem>();
	engine.AddSystem<GUISystem>();
#endif // !NDEBUG
//    engine.AddSystem<JiggleSystem>();

	EngineInit(engine);
	
	// Post-init pre-loop setup

	ShaderSystem::Use("standard");

	auto& reg = engine.GetRegistry();

	for (int i = 0; i < 10; i++)
	{
		auto entity = reg.create();
		auto& sprite = reg.emplace<Sprite>(entity);
		sprite.Use("dagger");
		sprite.m_Scale = 0.2f;
	}

	// Game loop starts here 

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
