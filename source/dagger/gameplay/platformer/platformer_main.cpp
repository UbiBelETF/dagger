#include "gameplay/platformer/platformer_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
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

#include "gameplay/platformer/platformer_controller.h"

using namespace dagger;

void Platformer::GameplaySystemsSetup(Engine& engine_)
{
	engine_.AddSystem<PlatformerControllerSystem>();
}

void Platformer::WorldSetup(Engine& engine_)
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    auto& reg = engine_.Registry();

    {
        auto entity1 = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity1);
        sprite.scale = { 1, 1 };
        sprite.position = { -10, 0, 1 };
        sprite.color = { 1, 0, 0, 1 };
        AssignSpriteTexture(sprite, "souls_like_knight_character:IDLE:idle1");

        auto& input = reg.emplace<InputReceiver>(entity1);
        input.contexts.push_back("Player");

        auto& anim = reg.emplace<Animator>(entity1);
        AnimatorPlay(anim, "souls_like_knight_character:IDLE");
    }

    {
        auto entity2 = reg.create();
        auto& sprite2 = reg.emplace<Sprite>(entity2);
        sprite2.scale = { 1, 1 };
        sprite2.position = { 10, 0, 0 };
        sprite2.color = { 0, 1, 0, 1 };
        AssignSpriteTexture(sprite2, "souls_like_knight_character:IDLE:idle1");
    }
}

