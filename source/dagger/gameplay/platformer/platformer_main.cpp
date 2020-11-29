#include "gameplay/platformer/platformer_main.h"


#include "core/core.h"
#include "core/engine.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/graphics/sprite_render.h"
#include "core/graphics/textures.h"
#include "core/graphics/animations.h"
#include "core/graphics/gui.h"
#include "tools/diagnostics.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/game/transforms.h"
#include "core/input/inputs.h"

#include "gameplay/platformer/platformer_collision.h"
#include "gameplay/platformer/platformer_controller.h"
#include "gameplay/platformer/platformer_combat.h"
#include "gameplay/platformer/parallax.h"
#include "gameplay/platformer/camera_focus.h"


using namespace dagger;
using namespace platformer;

void Platformer::GameplaySystemsSetup(Engine& engine_)
{
	engine_.AddSystem<PlatformerControllerSystem>();
    engine_.AddSystem<ParallaxSystem>();
    engine_.AddSystem<CameraFollowSystem>();
    engine_.AddSystem<PlatformerCombatSystem>();
    engine_.AddSystem<PlatformerCollisionsSystem>();
}

void SetCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 0, 0, 0 };
    camera->Update();
}

struct Character
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    PlatformerCharacter& character;

    static Character Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<PlatformerCharacter>(entity);

        return Character{ entity, sprite, anim, input, character };
    }

    static Character Create(
        String input_ = "", 
        ColorRGB color_ = { 1, 1, 1 }, 
        Vector2 position_ = { 0, 0 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();

        ATTACH_TO_FSM(CharacterControllerFSM, entity);

        auto chr = Character::Get(entity);

        chr.sprite.scale = { 1, 1 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "souls_like_knight_character:IDLE:idle1");
        AnimatorPlay(chr.animator, "souls_like_knight_character:IDLE");

        if(input_ != "")
            chr.input.contexts.push_back(input_);

        chr.character.speed = 50;

        return chr;
    }
};

void CreateBackdrop()
{
    auto& reg = Engine::Registry();
    auto* camera = Engine::GetDefaultResource<Camera>();

    /* Create terrain */ {
        auto back = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(back);
        
        AssignSprite(sprite, "EmptyWhitePixel");
        sprite.color = { 0, 0, 0, 1 };
        sprite.size = { 200, 200 };
        sprite.scale = { 10, 1 };
        sprite.position = { 0, -125, 1 };
    }

    /* Put background image */ {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "souls_like_knight_character:BACKGROUND:Background");
        sprite.position.z = 10;
    }

    /* Put grass */ {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& parallax = reg.get_or_emplace<Parallax>(entity);
        parallax.lastCameraPosition = camera->position;
        parallax.strength = 0.5f;

        AssignSprite(sprite, "souls_like_knight_character:BACKGROUND:Grass");
        sprite.position = { 0, -25, 5 };
    }

    /* Put trees */ {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& parallax = reg.get_or_emplace<Parallax>(entity);
        parallax.lastCameraPosition = camera->position;
        parallax.strength = 0.25f;

        AssignSprite(sprite, "souls_like_knight_character:BACKGROUND:Tree");
        sprite.position = { 0, 30, 7 };
    }
}



void Platformer::WorldSetup(Engine& engine_)
{
    SetCamera();
    CreateBackdrop();

    auto mainChar = Character::Create("ASDW", { 1, 1, 1 }, { -100, 0 });
    Engine::Registry().emplace<CameraFollowFocus>(mainChar.entity);

    auto backgroundBar1 = Engine::Registry().create();
    auto currentHealthBar1 = Engine::Registry().create();

    mainChar.chealth.backgroundBar = backgroundBar1;
    mainChar.chealth.currentHealthBar = currentHealthBar1;

    auto& backgroundSprite = Engine::Registry().emplace<Sprite>(mainChar.chealth.backgroundBar);

    AssignSprite(backgroundSprite, "EmptyWhitePixel");
    backgroundSprite.color = { 0, 0, 0, 1 };
    backgroundSprite.size = { 50, 5 };
    backgroundSprite.scale = { 1, 1 };
    backgroundSprite.position = { -100, 125, 2 };

    auto& frontSprite = Engine::Registry().emplace<Sprite>(mainChar.chealth.currentHealthBar);

    AssignSprite(frontSprite, "EmptyWhitePixel");
    frontSprite.color = { 1, 0, 0, 1 };
    frontSprite.size = { 50, 5 };
    frontSprite.scale = { 1, 1 };
    frontSprite.position = {-100, 125, 1 };

    auto* camera = Engine::GetDefaultResource<Camera>();

    auto& backgroundParallax = Engine::Registry().emplace<Parallax>(mainChar.chealth.backgroundBar);
    backgroundParallax.lastCameraPosition = camera->position;
    backgroundParallax.strength = 1.0f;

    auto& frontParallax = Engine::Registry().emplace<Parallax>(mainChar.chealth.currentHealthBar);
    frontParallax.lastCameraPosition = camera->position;
    frontParallax.strength = 1.0f;


    auto sndChar = Character::Create("Arrows", { 1, 0, 0 }, { 100, 0 });
    Engine::Registry().emplace<CameraFollowFocus>(sndChar.entity);

    auto backgroundBar2 = Engine::Registry().create();
    auto currentHealthBar2 = Engine::Registry().create();

    sndChar.chealth.backgroundBar = backgroundBar2;
    sndChar.chealth.currentHealthBar = currentHealthBar2;

    auto& backgroundSprite2 = Engine::Registry().emplace<Sprite>(sndChar.chealth.backgroundBar);

    AssignSprite(backgroundSprite2, "EmptyWhitePixel");
    backgroundSprite2.color = { 0, 0, 0, 1 };
    backgroundSprite2.size = { 50, 5 };
    backgroundSprite2.scale = { 1, 1 };
    backgroundSprite2.position = { 100, 125, 2 };

    auto& frontSprite2 = Engine::Registry().emplace<Sprite>(sndChar.chealth.currentHealthBar);

    AssignSprite(frontSprite2, "EmptyWhitePixel");
    frontSprite2.color = { 1, 0, 0, 1 };
    frontSprite2.size = { 50, 5 };
    frontSprite2.scale = { 1, 1 };
    frontSprite2.position = { 100, 125, 1 };

    auto& backgroundParallax2 = Engine::Registry().emplace<Parallax>(sndChar.chealth.backgroundBar);
    backgroundParallax2.lastCameraPosition = camera->position;
    backgroundParallax2.strength = 1.0f;

    auto& frontParallax2 = Engine::Registry().emplace<Parallax>(sndChar.chealth.currentHealthBar);
    frontParallax2.lastCameraPosition = camera->position;
    frontParallax2.strength = 1.0f;
}


