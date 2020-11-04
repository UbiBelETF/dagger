
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
#include "core/game/scene_management.h"
#include "tools/diagnostics.h"
#include "tools/console.h"
#include "tools/toolmenu.h"

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

using namespace dagger;

void CoreSystemsSetup(Engine &engine_, GameRenderConfig renderConfig_)
{
    engine_.AddSystem<WindowSystem>(renderConfig_);
    engine_.AddSystem<InputSystem>();
    engine_.AddSystem<ShaderSystem>();
    engine_.AddSystem<TextureSystem>();
    engine_.AddSystem<SpriteRenderSystem>();
    engine_.AddSystem<AnimationSystem>();
    engine_.AddSystem<TransformSystem>();
#if !defined(NDEBUG)
    engine_.AddSystem<DiagnosticSystem>();
    engine_.AddSystem<GUISystem>();
    engine_.AddSystem<ToolMenuSystem>();
#endif //!defined(NDEBUG)
}

void GameplaySystemsSetup(Engine& engine_)
{
    SceneManagement::SetupSystems(engine_);
}

void WorldSetup(Engine& engine_)
{
    SceneManagement::SetupWorld(engine_);
}

int main(int argc_, char** argv_)
{
    // scene choice
    SceneManagement::SetCurrentScene(EScene::PingPongGame);

    // todo: read configs from xml/json
    // render settings
    GameRenderConfig renderConfig;
    renderConfig.fullscreen = false;
    renderConfig.lockScreenResize = true;
    renderConfig.windowHeight = 600;
    renderConfig.windowWidth = 800;

    Engine engine;
    CoreSystemsSetup(engine, renderConfig);
    GameplaySystemsSetup(engine);

    engine.EngineInit();
    WorldSetup(engine);

    while (engine.Up())
        engine.EngineLoop();

    engine.EngineStop();

    return 0;
}
