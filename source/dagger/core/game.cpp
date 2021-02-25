#include "game.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/audio.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "tools/diagnostics.h"
#include "gameplay/team_game/tiles.h"

void dagger::Game::CoreSystemsSetup(Engine& engine_)
{
    engine_.AddSystem<AudioSystem>();
    engine_.AddSystem<WindowSystem>();
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
    engine_.AddSystem<TilemapLoadingSystem>();
#endif //!defined(NDEBUG)
}