
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

#include <glm/glm.hpp>

using namespace dagger;

int main(int argc_, char** argv_)
{
	srand(time(0));
	spdlog::set_level(spdlog::level::trace);

	Engine engine;

	engine.AddSystem<WindowSystem>(1200, 800);
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
		sprite.UseShader("standard");
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
