#include "gameplay/platformer/platformer.h"

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

#include "gameplay/common/character_controller.h"

using namespace dagger;

void Platformer::GameplaySystemsSetup(Engine& engine_)
{
//	engine_.AddSystem<CharacterControllerSystem>();
}

void Platformer::WorldSetup(Engine& engine_)
{
    ShaderSystem::Use("standard");

    Camera camera;
    camera.mode = ECameraMode::FixedResolution;
    camera.size = { 800, 600 };
    camera.zoom = 0.17f;
    Engine::Dispatcher().trigger<Camera>(camera);

    auto& reg = engine_.Registry();

    {
        auto entity1 = reg.create();
        auto& sprite1 = reg.emplace<Sprite>(entity1);
        sprite1.scale = { 10, 10 };
        sprite1.position = { 0, 0, 0 };
        AssignSpriteTexture(sprite1, "souls_like_knight_character:IDLE:idle1");

        auto& anim1 = reg.emplace<Animator>(entity1);
        AnimatorPlay(anim1, "souls_like_knight_character:IDLE");
    }

    {
        auto entity2 = reg.create();

        auto& sprite2 = reg.emplace<Sprite>(entity2);
        sprite2.scale = { 3, 3 };
        sprite2.color = { 0, 1, 0, 1 };
        sprite2.position = { -2, 0, 0.5f };
        AssignSpriteTexture(sprite2, "souls_like_knight_character:SKY_ATTACK:sky_attack1");

        auto& anim2 = reg.emplace<Animator>(entity2);
        AnimatorPlay(anim2, "souls_like_knight_character:SKY_ATTACK");
    }

    {
        auto entity3 = reg.create();
        auto& sprite3 = reg.emplace<Sprite>(entity3);
        sprite3.scale = { 5, 5 };
        sprite3.color = { 1, 0, 0, 1 };
        sprite3.position = { 0.5f, 0, 1 };
        AssignSpriteTexture(sprite3, "souls_like_knight_character:ATTACK:attack1");

        auto& anim3 = reg.emplace<Animator>(entity3);
        AnimatorPlay(anim3, "souls_like_knight_character:ATTACK");
    }
}

