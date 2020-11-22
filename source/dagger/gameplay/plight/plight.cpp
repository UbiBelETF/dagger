#include "plight.h"

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

#include "gameplay/plight/topdown_controller.h"

using namespace dagger;
using namespace plight;

struct PlightCharacter
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    TopdownCharacter& character;

    static PlightCharacter Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<TopdownCharacter>(entity);
        return PlightCharacter{ entity, sprite, anim, input, character };
    }

    static PlightCharacter Create(
        String input_ = "",
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 0, 0 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto chr = PlightCharacter::Get(entity);

        chr.sprite.scale = { 1, 1 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        AssignSpriteTexture(chr.sprite, "Plight:big_deamon:IDLE:big_demon_idle_anim_f0");
        AnimatorPlay(chr.animator, "Plight:big_deamon:IDLE");

        if (input_ != "")
            chr.input.contexts.push_back(input_);

        chr.character.speed = 50;

        return chr;
    }
};

void Plight::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<TopdownControllerSystem>();
}

void Plight::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 1;
    camera->position = { 0, 0, 0 };
    camera->Update();

    plight::SetupWorld_test1(engine_);
}

void plight::SetupWorld(Engine &engine_)
{
}

void setUpBackground(Engine& engine_) {
    auto& reg = engine_.Registry();
    for (int i = -50; i < 50; i++)
    {
        for (int j = -50; j < 50; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSpriteTexture(sprite, fmt::format("Plight:floor:floor_{}", 1 + (rand() % 8)));
            sprite.position = { i * 16, j * 16, 1 };
        }
    }

}

void plight::SetupWorld_test1(Engine& engine_) {
   
    setUpBackground(engine_);

    auto mainChar = PlightCharacter::Create("ASDW_topdown", { 1, 1, 1 }, { 0, 0 });

}



