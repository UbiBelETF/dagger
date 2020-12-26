#include "gameplay/team_game/team_game_main.h"
#include "gameplay/team_game/hero_controller.h"
#include "gameplay/team_game/slime_controller.h"
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
#include "core/graphics/text.h"
#include "tilemap_system.h"
#include "tilemap_legends.h"
#include "health_system.h"
#include "gameplay/team_game/collision.h"
#include"gameplay/team_game/ai_system.h"
#include "gameplay/team_game/camera_follow.h"
#include "level_generator.h"
#include "attack_system.h"
using namespace dagger;
using namespace team_game;


void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<TilemapSystem>();
    engine_.AddSystem<CollisionSystem>();
    engine_.AddSystem<TeamGameControllerSystem>();
    engine_.AddSystem<TeamGameSlimeControllerSystem>();
    engine_.AddSystem<AiSystem>();
    engine_.AddSystem<HealthSystem>();
    engine_.AddSystem<TeamCameraFollowSystem>();
    engine_.AddSystem<AttackSystem>();

}
void SetCameraTeam()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;

    camera->size = { 800, 600 };
    camera->zoom = 3;
    camera->position = { 0, 0, 0 };
    camera->Update();
}
struct MainCharacter
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    TeamGameCharacter& character;
    Collision& col;
    Transform& tm;
    Health& hp;
    Attack& att;
    static MainCharacter Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<TeamGameCharacter>(entity);
        auto& col = reg.get_or_emplace<Collision>(entity);
        auto& tm = reg.get_or_emplace<Transform>(entity);
        auto& hp = reg.get_or_emplace<Health>(entity);
        auto& att = reg.get_or_emplace<Attack>(entity);
        return MainCharacter{ entity, sprite, anim, input, character, col, tm,hp,att };
    }

    static MainCharacter Create(
        String input_ = "",
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 0, 0 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto chr = MainCharacter::Get(entity);
        

        ATTACH_TO_FSM(HeroControllerFSM, entity);
        
        chr.sprite.scale = { 3, 3 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "spritesheets:chara_hero:hero_idle_anim:1");
        AnimatorPlay(chr.animator, "chara_hero:hero_idle");

        if (input_ != "")
            chr.input.contexts.push_back(input_);

        chr.character.speed = 100;

        //Collision setup:        

        chr.col.size = { 16, 16 };

        chr.tm.position = { position_ , 0.0f };
        reg.emplace<CollisionType::Character>(entity);

        reg.emplace<TeamCameraFollowFocus>(entity);

        return chr;
    }
};

void TeamGame::WorldSetup(Engine &engine_)
{
    //TODO: Setup world
    SetCameraTeam();
    auto mainChar = MainCharacter::Create("ASDW", { 1, 1, 1 }, { 0, 0 });
    auto& reg = Engine::Registry();


    Tilemap_legends tilemap_legends;
    LevelGenerator levelGen;
   /* Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ "levels/floor_level_1.map", &tilemap_legends.legends.at("floors"),0,0 });
    Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ "levels/objects_level_1.map",&tilemap_legends.legends.at("objects"),0,0 });
    Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ "levels/creatures_level_1.map", &tilemap_legends.legends.at("creatures"),0,0 });*/
    levelGen.GenerateLevel(4, 2);

    
    team_game::SetupWorld(engine_);
}

void team_game::SetupWorld(Engine &engine_)
{
   
}


