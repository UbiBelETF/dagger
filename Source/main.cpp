
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

	engine.AddSystem<WindowSystem>(1200, 800);
    engine.AddSystem<TextureSystem>();
	engine.AddSystem<SpriteRenderSystem>();

#ifndef NDEBUG
	engine.AddSystem<GUISystem>();
	engine.AddSystem<DiagnosticSystem>();
	engine.AddSystem<ConsoleSystem>();
	engine.AddSystem<ToolMenuSystem>();
#endif // !NDEBUG

    // gameplay systems
    engine.AddSystem<JiggleSystem>();

	EngineInit(engine);

	auto &reg = engine.GetRegistry();

	auto textures = Engine::Res<Texture>();

	for (int i = 0; i < 1; i++)
	{
		auto entity = reg.create();
		auto& sprite = reg.emplace<Sprite>(entity);
		sprite.m_Color.R = 1.0f;
		sprite.m_Color.G = 1.0f;
		sprite.m_Color.B = 1.0f;
		sprite.m_Color.A = 1.0f;
		sprite.m_Position.Z = 0.0001f * Engine::ms_EntityId++;
		sprite.m_Image = TextureSystem::Get("rayman");
	}

	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
