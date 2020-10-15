
#include "optimization.h"
#include "engine.h"
#include "textures.h"
#include "texture.h"
#include "window.h"
#include "diag.h"
#include "gui.h"
#include "console.h"
#include "sprite_render.h"
#include "toolmenu.h"
#include "jiggle.h"

#include <spdlog/spdlog.h>
#include <filesystem>

using namespace dagger;

int main(int argc_, char** argv_)
{
	srand(time(NULL));
	spdlog::set_level(spdlog::level::trace);

	Engine engine;

	engine.AddSystem<WindowSystem>();
	engine.AddSystem<ShaderSystem>();
	engine.AddSystem<TextureSystem>();
	engine.AddSystem<SpriteRenderSystem>();
	engine.AddSystem<DiagnosticSystem>();
#ifndef NDEBUG
	engine.AddSystem<GUISystem>();
	engine.AddSystem<ConsoleSystem>();
	engine.AddSystem<ToolMenuSystem>();
#endif // !NDEBUG
    engine.AddSystem<JiggleSystem>();

	EngineInit(engine);
	
	// Post-init pre-loop setup

	ShaderSystem::Use("standard");

	auto& reg = engine.GetRegistry();

	for (int i = 0; i < 1000; i++)
	{
		auto entity = reg.create();
		auto& sprite = reg.emplace<Sprite>(entity);
		sprite.m_Color.R = 1.0f;
		sprite.m_Color.G = 1.0f;
		sprite.m_Color.B = 1.0f;
		sprite.m_Color.A = 1.0f;
		sprite.m_Position.Z = 0.0001f * Engine::ms_EntityId++;
		sprite.m_Image = TextureSystem::Get("goblin");
	}

	// Game loop starts here 

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
