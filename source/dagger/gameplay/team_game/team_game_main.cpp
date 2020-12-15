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
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/player_controller.h"

#include "tilemap_entities.h"
#include "gameplay/team_game/shoot.h"
#include "gameplay/team_game/enemy.h"
#include "core/graphics/text.h"



using namespace dagger;
using namespace lab;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddSystem<SimpleCollisionsSystem>();
    engine_.AddSystem<PlayerControllerSystem>();
    engine_.AddSystem<EnemySystem>();
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
    Transform& transform;
    NextLvl& currentLvl;
    PlayerCharacter& character;

    static Player Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& transform = reg.get_or_emplace<Transform>(entity);
        auto& currentLvl = reg.get_or_emplace<NextLvl>(entity);
        auto& character = reg.get_or_emplace<PlayerCharacter>(entity);
        return Player{ entity, sprite, anim, input,transform,currentLvl, character };
    }

    static Player Create(
        String input_ = "", 
        ColorRGB color_ = { 1, 1, 1 }, 
        Vector2 position_ = { 0, 0 })
        
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto chr = Player::Get(entity);

        chr.sprite.scale = { 1, 1 };
        chr.sprite.position = { position_, 0.0f };
        chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "main_character:idle:idle1");
        AnimatorPlay(chr.animator, "main_character:idle");
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = {chr.sprite.size.x/2, chr.sprite.size.y/2};

        chr.transform.position = chr.sprite.position;

        if(input_ != "")
        {
            chr.input.contexts.push_back(input_);
        }
           

        chr.character.speed = 50;
        chr.currentLvl.id=0;
        chr.currentLvl.room[0]="tilemaps/lab/lab.map";
        chr.currentLvl.room[1]="tilemaps/lab/hallway.map";
        chr.currentLvl.room[2]="tilemaps/lab/room2.map";

        chr.currentLvl.legend['#'] = CreateObjectFunction("spritesheets:lab:wall_2",30,true,-1,100.0f,"");//CreateWallTop
        chr.currentLvl.legend['='] = CreateObjectFunction("spritesheets:lab:wall_5",30,true,-1,100.0f,"");//CreateWallUpPart;
        chr.currentLvl.legend['-'] = CreateObjectFunction("spritesheets:lab:wall_top_second_left",30,true,-1,100.0f,"");//CreateWallDownPart;
        chr.currentLvl.legend['.'] = CreateObjectFunction("create_floor",30,false,-1,100.0f,"");
        chr.currentLvl.legend['|'] = CreateObjectFunction("spritesheets:lab:wall_6",30,true,-1,100.0f,"");//CreateSideWallRight;  
        chr.currentLvl.legend[':'] = CreateObjectFunction("spritesheets:lab:wall_4",30,true,-1,100.0f,"");//CreateSideWallLeft;
        chr.currentLvl.legend['1'] = CreateObjectFunction("spritesheets:lab:wall_1",30,true,-1,100.0f,"");//CreateWall1;
        chr.currentLvl.legend['3'] = CreateObjectFunction("spritesheets:lab:wall_3",30,true,-1,100.0f,"");//CreateWall3;
        chr.currentLvl.legend['0'] = CreateObjectFunction("",30,false,-1,100.0f,"");//Empty;
        chr.currentLvl.legend['F'] = CreateObjectFunction("lab:door:lab anims_Animation 5_00",30,true,2,107.1f,"lab:door");//Door;
        chr.currentLvl.legend['Q'] = CreateObjectFunction("spritesheets:lab:wall_bottom_1",30,true,-1,100.0f,"");//CreateWallBottom1;
        chr.currentLvl.legend['W'] = CreateObjectFunction("spritesheets:lab:wall_bottom_6",30,true,-1,100.0f,"");//CreateWallBottom6;
        chr.currentLvl.legend['8'] = CreateObjectFunction("spritesheets:lab:floor_1",30,true,1,100.0f,"");//Hall
        chr.currentLvl.legend['Z'] = CreateObjectFunction("spritesheets:lab:wall_bottom_5_blank",30,false,-1,100.0f,""); //BlankWall
        chr.currentLvl.legend['9'] = CreateObjectFunction("spritesheets:lab:floor_1",30,true,0,100.0f,"");//MainRoom

        TilemapLegend legend=chr.currentLvl.legend;
        Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/lab/lab.map", &legend });

        auto& hl = Engine::Registry().emplace<Text>(entity);
        hl.alignment={ TextAlignment::RIGHT };
        hl.spacing = 0.5f;
        hl.Set("pixel-font", "100/100",{10,-95,0},{10,10});

        return chr;
    }
};
    

void lab::SetupWorld(Engine &engine_)
{
    auto& reg = engine_.Registry();
       Vector2 scale(1, 1);              

     auto mainChar = Player::Create("ASDW", { 1, 1, 1 }, { -100, 0 });
     //Engine::Registry().destroy(mainChar.entity);

      
    for (int i = 0; i < 3; i++)
    {
        auto entity = reg.create();
        auto& bandit = reg.emplace<Bandit>(entity);
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Bandit");
        sprite.scale = { 1, 1 };
        if (i == 0)
        sprite.position = { 100, 0, 0.0f };

        if (i == 1)
        sprite.position = { 100, 50, 0.0f };

        if (i == 2)
            sprite.position = { 100, -50, 0.0f };

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = sprite.position;

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = { 0.5f, 10 };
    }


}

