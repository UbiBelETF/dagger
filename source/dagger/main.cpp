
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
#include "core/game/transforms.h"
#include "tools/diagnostics.h"
#include "tools/console.h"
#include "tools/toolmenu.h"

#include "gameplay/examples/character_controller.h"
#include "gameplay/ping_pong/ping_pong_main.h"

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

void GameplaySystemsSetup(Engine& engine)
{
    //example1::SetupSystems(engine);
    PingPong::SetupSystems(engine);
}

void WorldSetup(Engine& engine)
{
    //example1::SetupWorld(engine);
    PingPong::SetupWorld(engine);
}

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
