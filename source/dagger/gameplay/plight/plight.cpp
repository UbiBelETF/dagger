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


#include "gameplay/plight/plight_combat.h"
#include "gameplay/plight/plight_collisions.h"
#include "gameplay/plight/plight_state_idle.h"
#include "gameplay/plight/plight_state_running.h"


using namespace dagger;
using namespace plight;

struct PlightCharacter
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    PlightCollision& col;
    Transform& transform;
    CombatStats& cstats;
    

    static PlightCharacter Get(Entity entity_)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity_);
        auto& anim = reg.get_or_emplace<Animator>(entity_);
        auto& input = reg.get_or_emplace<InputReceiver>(entity_);
        auto& col = reg.get_or_emplace<PlightCollision>(entity_);
        auto& transform = reg.get_or_emplace<Transform>(entity_);
        auto& cstats = reg.get_or_emplace<CombatStats>(entity_);
        return PlightCharacter{ entity_, sprite, anim, input,col,transform,cstats};
    }

    static PlightCharacter Create(
        String input_ = "",
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 0, 0 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto chr = PlightCharacter::Get(entity);

        reg.emplace<StateIdle>(entity); // Start state is state idle

        chr.sprite.scale = { 1, 1 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        chr.col.size.x = 16;
        chr.col.size.y = 16;

        chr.transform.position = { position_, 0.0f };

        AssignSprite(chr.sprite, "Plight:big_deamon:IDLE:big_demon_idle_anim_f0");
        AnimatorPlay(chr.animator, "Plight:big_deamon:IDLE");

        if (input_ != "")
            chr.input.contexts.push_back(input_);


        return chr;
    }
};

void Plight::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<PlightCollisionsSystem>();
    engine_.AddSystem<PlightCombatSystem>();
    engine_.AddSystem<PlightIdleStateSystem>();
    engine_.AddSystem<PlightRunningStateSystem>();

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

    plight::SetupWorld_CombatSystem(engine_);
}

void plight::SetupWorld(Engine &engine_)
{
}

void setUpBackground(Engine& engine_) {
    auto& reg = engine_.Registry();
    for (int i = -30; i < 30; i++)
    {
        for (int j = -30; j < 30; j++)
        {
            auto entity = reg.create();
            auto& sprite = reg.emplace<Sprite>(entity);
            AssignSprite(sprite, fmt::format("Plight:floor:floor_{}", 1 + (rand() % 8)));
            sprite.position = { i * 16, j * 16, 10 };
        }
    }

}

void plight::SetupWorld_test1(Engine& engine_) {
   
    setUpBackground(engine_);

    auto mainChar = PlightCharacter::Create("ASDW_topdown", { 1, 1, 1 }, { 0, 0 });

}

void plight::SetupWorld_CombatSystem(Engine& engine_){
    setUpBackground(engine_);

    auto mainChar = PlightCharacter::Create("ASDW_topdown", { 1, 1, 1 }, { -100, 0 });

    auto backgroundHealthBar1 = Engine::Registry().create();
    auto currentHealthBar1 = Engine::Registry().create();

    auto backgroundStaminaBar1 = Engine::Registry().create();
    auto currentStaminaBar1 = Engine::Registry().create();

    mainChar.cstats.backgroundHealthBar = backgroundHealthBar1;
    mainChar.cstats.currentHealthBar = currentHealthBar1;
    mainChar.cstats.backgroundStaminaBar = backgroundStaminaBar1;
    mainChar.cstats.currentStaminaBar = currentStaminaBar1;

    auto& backgroundSprite = Engine::Registry().emplace<Sprite>(mainChar.cstats.backgroundHealthBar);

    AssignSprite(backgroundSprite, "EmptyWhitePixel");
    backgroundSprite.color = { 0, 0, 0, 1 };
    backgroundSprite.size = { 50, 5 };
    backgroundSprite.scale = { 1, 1 };
    backgroundSprite.position = { -100, 125, 2 };

    auto& frontSprite = Engine::Registry().emplace<Sprite>(mainChar.cstats.currentHealthBar);

    AssignSprite(frontSprite, "EmptyWhitePixel");
    frontSprite.color = { 1, 0, 0, 1 };
    frontSprite.size = { 50, 5 };
    frontSprite.scale = { 1, 1 };
    frontSprite.position = { -100, 125, 1 };

    auto& backgroundStaminaSprite = Engine::Registry().emplace<Sprite>(mainChar.cstats.backgroundStaminaBar);

    AssignSprite(backgroundStaminaSprite, "EmptyWhitePixel");
    backgroundStaminaSprite.color = { 0, 0, 0, 1 };
    backgroundStaminaSprite.size = { 50, 5 };
    backgroundStaminaSprite.scale = { 1, 1 };
    backgroundStaminaSprite.position = { -100, 115, 2 };

    auto& frontStaminaSprite = Engine::Registry().emplace<Sprite>(mainChar.cstats.currentStaminaBar);

    AssignSprite(frontStaminaSprite, "EmptyWhitePixel");
    frontStaminaSprite.color = { 0, 1, 0, 1 };
    frontStaminaSprite.size = { 50, 5 };
    frontStaminaSprite.scale = { 1, 1 };
    frontStaminaSprite.position = { -100, 115, 1 };



    auto sndChar = PlightCharacter::Create("arrows_topdown", { 1, 0, 0 }, { 100, 0 });

    auto backgroundHealthBar2 = Engine::Registry().create();
    auto currentHealthBar2 = Engine::Registry().create();
    auto backgroundStaminaBar2 = Engine::Registry().create();
    auto currentStaminaBar2 = Engine::Registry().create();

    sndChar.cstats.backgroundHealthBar = backgroundHealthBar2;
    sndChar.cstats.currentHealthBar = currentHealthBar2;
    sndChar.cstats.backgroundStaminaBar = backgroundStaminaBar2;
    sndChar.cstats.currentStaminaBar = currentStaminaBar2;

    auto& backgroundSprite2 = Engine::Registry().emplace<Sprite>(sndChar.cstats.backgroundHealthBar);

    AssignSprite(backgroundSprite2, "EmptyWhitePixel");
    backgroundSprite2.color = { 0, 0, 0, 1 };
    backgroundSprite2.size = { 50, 5 };
    backgroundSprite2.scale = { 1, 1 };
    backgroundSprite2.position = { 100, 125, 2 };

    auto& frontSprite2 = Engine::Registry().emplace<Sprite>(sndChar.cstats.currentHealthBar);

    AssignSprite(frontSprite2, "EmptyWhitePixel");
    frontSprite2.color = { 1, 0, 0, 1 };
    frontSprite2.size = { 50, 5 };
    frontSprite2.scale = { 1, 1 };
    frontSprite2.position = { 100, 125, 1 };

    auto& backgroundStaminaSprite2 = Engine::Registry().emplace<Sprite>(sndChar.cstats.backgroundStaminaBar);

    AssignSprite(backgroundStaminaSprite2, "EmptyWhitePixel");
    backgroundStaminaSprite2.color = { 0, 0, 0, 1 };
    backgroundStaminaSprite2.size = { 50, 5 };
    backgroundStaminaSprite2.scale = { 1, 1 };
    backgroundStaminaSprite2.position = { 100, 115, 2 };

    auto& frontStaminaSprite2 = Engine::Registry().emplace<Sprite>(sndChar.cstats.currentStaminaBar);

    AssignSprite(frontStaminaSprite2, "EmptyWhitePixel");
    frontStaminaSprite2.color = { 0, 1, 0, 1 };
    frontStaminaSprite2.size = { 50, 5 };
    frontStaminaSprite2.scale = { 1, 1 };
    frontStaminaSprite2.position = { 100, 115, 1 };
}



