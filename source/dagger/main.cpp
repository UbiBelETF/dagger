
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
#include "tools/diagnostics.h"
#include "tools/console.h"
#include "tools/toolmenu.h"

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
#if !defined(NDEBUG)
    engine.AddSystem<DiagnosticSystem>();
    engine.AddSystem<GUISystem>();
    engine.AddSystem<ToolMenuSystem>();
#endif //!defined(NDEBUG)
}

class PlayerInfoSystem : public System
{
    String SystemName() override {
        return "Player Info System";
    }

    void Initialize(entt::registry& reg_, entt::entity entity_)
    {
        auto& receiver_ = reg_.get<InputReceiver>(entity_);
        receiver_.values["Player:run"] = 0;
        receiver_.values["Player:jump"] = 0;
        receiver_.values["Player:down"] = 0;
        receiver_.values["Player:heavy"] = 0;
        receiver_.values["Player:light"] = 0;
        receiver_.values["Player:use"] = 0;
    }

    inline void SpinUp() override
    {
        Engine::Registry().on_construct<InputReceiver>().connect<&PlayerInfoSystem::Initialize>(this);
    }

    inline void Run() override
    {
        Engine::Registry().view<InputReceiver>().each([](InputReceiver receiver_)
            {
                for (auto& [k, v] : receiver_.values)
                {
                    if (v != 0)
                    {
                        Logger::critical("{}: {}", k, v);
                    }
                }
            });
    }
};

void GameplaySystemsSetup(Engine &engine)
{
    engine.AddSystem<PlayerInfoSystem>();
}

void PostInitWorld(Engine &engine)
{
    ShaderSystem::Use("standard");

    auto& reg = engine.GetRegistry();
    auto entity = reg.create();
    auto& sprite = reg.emplace<Sprite>(entity);
    sprite.scale = 500.f;
    auto& anim = reg.emplace<Animator>(entity);
    AnimatorPlay(anim, "souls_like_knight_character:ATTACK");
    auto& input = reg.emplace<InputReceiver>(entity);
    input.contexts.push_back("Player");
}

int main(int argc_, char** argv_)
{
	srand(time(0));
	Logger::set_level(Logger::level::trace);

	Engine engine;
    CoreSystemsSetup(engine);
    GameplaySystemsSetup(engine);

	EngineInit(engine);

    PostInitWorld(engine);

	// Game loop starts here 
	while (engine.Up())
	{
		EngineLoop(engine);
	}

	EngineStop(engine);

	return 0;
}
