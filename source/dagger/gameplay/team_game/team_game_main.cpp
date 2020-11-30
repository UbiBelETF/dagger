#include "team_game_main.h"
#include "tiles.h"

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

#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/player_controller.h"

#include "tilemap_entities.h"
#include "gameplay/team_game/shoot.h"


using namespace dagger;
using namespace lab;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<PlayerControllerSystem>();
    engine_.AddSystem<ShootingSystem>();
    engine_.AddPausableSystem<TransformSystem>();
}

void TeamGame::WorldSetup(Engine &engine_)
{
    ShaderSystem::Use("standard");

    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 3;
    camera->position = { 0, 0, 0 };
    camera->Update();

    lab::SetupWorld(engine_);
}
struct Player
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    PlayerCharacter& character;
    Transform& transform;

    static Player Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<PlayerCharacter>(entity);
        auto& transform = reg.get_or_emplace<Transform>(entity);
        return Player{ entity, sprite, anim, input, character, transform };
    }

    static Player Create(
        String input_ = "", 
        ColorRGB color_ = { 1, 1, 1 }, 
        Vector2 position_ = { 0, 0 })


    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto chr = Player::Get(entity);


        
        chr.transform.position = { position_, 0.0f };

        chr.sprite.scale = { 1, 1 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "main_character:idle:idle1");
        AnimatorPlay(chr.animator, "main_character:idle");
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = chr.sprite.size;

        if(input_ != "")
            chr.input.contexts.push_back(input_);

        chr.character.speed = 50;

        return chr;
    }
};
    

void lab::SetupWorld(Engine &engine_)
{
    auto& reg = engine_.Registry();

    TilemapLegend legend;
    legend['#'] = &CreateWallTop;
    legend['='] = &CreateWallUpPart;
    legend['-'] = &CreateWallDownPart;
    legend['.'] = &CreateFloor;
    legend['|'] = &CreateSideWallRight;  
    legend[':'] = &CreateSideWallLeft;
    legend['1'] = &CreateWall1;
    legend['3'] = &CreateWall3;
    
    Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/lab/lab.map", &legend }); 

    float zPos = 1.f;

    /*
    {
        
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "logos:dagger");
        float ratio = sprite.size.y / sprite.size.x;
        sprite.size = { 500 / ratio, 500  };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = { 0, 0, zPos };

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = sprite.size;
    } 

    */


    auto mainChar = Player::Create("ASDW", { 1, 1, 1 }, { -100, 0 });

}

