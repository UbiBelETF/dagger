#include "gameplay/team_game/team_game_main.h"
#include "gameplay/team_game/tiles.h"

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
#include "gameplay/team_game/player_controller_fsm.h"

#include "gameplay/team_game/tilemap_entities.h"
#include "gameplay/team_game/shoot.h"
#include "gameplay/team_game/enemy.h"
#include "core/graphics/text.h"
#include "character_controller_fsm.h"



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
        ATTACH_TO_FSM(CharacterFSM,entity);
        auto chr = Player::Get(entity);

        chr.sprite.scale = { 1, 1 };
        chr.sprite.position = { position_, 10 };
        chr.sprite.color = { color_, 1.0f };

        AssignSprite(chr.sprite, "main_character:idle:idle1");
        AnimatorPlay(chr.animator, "main_character:idle");
        auto& col = reg.emplace<SimpleCollision>(entity);
        //col.size = {chr.sprite.size.x/2, chr.sprite.size.y/2};
        col.size = { 12, 20 };

        chr.transform.position = chr.sprite.position;

        if(input_ != "")
        {
            chr.input.contexts.push_back(input_);
        }

        auto gunEntity = reg.create();
        auto& sprite = reg.emplace<Sprite>(gunEntity);
        sprite.scale = { 0.9f, 0.9f };
        AssignSprite(sprite, "Gun");
        auto& transform = reg.emplace<Transform>(gunEntity);
        transform.position = { position_, 0.0f };
        auto& gun = reg.emplace<Gun>(gunEntity);

        chr.character.speed = 50;
        chr.currentLvl.id=0;
        chr.currentLvl.room[0]="tilemaps/lab/lab.map";
        chr.currentLvl.room[1]="tilemaps/lab/hallway.map";
        chr.currentLvl.room[2]="tilemaps/lab/room2.map";
        chr.currentLvl.room[3]="tilemaps/lab/room3.map";
        chr.currentLvl.room[4]="tilemaps/lab/room4.map";
        chr.currentLvl.room[5]="tilemaps/lab/room5.map";
        chr.currentLvl.room[6]="tilemaps/lab/hallway_up.map";
        chr.currentLvl.room[7]="tilemaps/lab/room_boss.map";
        chr.currentLvl.addons[0]="tilemaps/lab/addons_lab.map";
        chr.currentLvl.addons[1]="tilemaps/lab/addons_hallway.map";
        chr.currentLvl.addons[2]="tilemaps/lab/addons_room2.map";
        chr.currentLvl.addons[3]="tilemaps/lab/addons_room3.map";
        chr.currentLvl.addons[4]="tilemaps/lab/addons_room4.map";
        chr.currentLvl.addons[5]="tilemaps/lab/addons_room5.map"; 
        chr.currentLvl.addons[6]="tilemaps/lab/addons_hallway_up.map"; 
        chr.currentLvl.addons[7]="tilemaps/lab/addons_boss.map"; 



        chr.currentLvl.legend['#'] = CreateObjectFunction("spritesheets:lab:wall_2",30,true,-1,100.0f,"",true,0);//CreateWallTop
        chr.currentLvl.legend['='] = CreateObjectFunction("spritesheets:lab:wall_5",30,true,-1,100.0f,"",true,0);//CreateWallUpPart;
        chr.currentLvl.legend['-'] = CreateObjectFunction("spritesheets:lab:wall_top_second_left",30,true,-1,100.0f,"",true,0);//CreateWallDownPart;
        chr.currentLvl.legend['.'] = CreateObjectFunction("create_floor",30,false,-1,100.0f,"",false,0);
        chr.currentLvl.legend['|'] = CreateObjectFunction("spritesheets:lab:wall_6",30,true,-1,100.0f,"",true,0);//CreateSideWallRight;  
        chr.currentLvl.legend[':'] = CreateObjectFunction("spritesheets:lab:wall_4",30,true,-1,100.0f,"", true, 0);//CreateSideWallLeft;
        chr.currentLvl.legend['1'] = CreateObjectFunction("spritesheets:lab:wall_1",30,true,-1,100.0f,"", true, 0);//CreateWall1;
        chr.currentLvl.legend['3'] = CreateObjectFunction("spritesheets:lab:wall_3",30,true,-1,100.0f,"", true, 0);//CreateWall3;
        chr.currentLvl.legend['5'] = CreateObjectFunction("spritesheets:lab:wall_15",30,true,-1,100.0f,"",true,0);//CreateWall15;
        chr.currentLvl.legend['0'] = CreateObjectFunction("",30,false,-1,100.0f,"",false,0);//Empty;
        
        chr.currentLvl.legend['Q'] = CreateObjectFunction("spritesheets:lab:wall_bottom_1",30,true,-1,100.0f,"",true,0);//CreateWallBottom1;
        chr.currentLvl.legend['W'] = CreateObjectFunction("spritesheets:lab:wall_bottom_6",30,true,-1,100.0f,"",true,0);//CreateWallBottom6;
        chr.currentLvl.legend['4'] = CreateObjectFunction("spritesheets:lab:wall_9",30,true,-1,100.0f,"", true, 0);//CreateWall9;
        chr.currentLvl.legend['6'] = CreateObjectFunction("spritesheets:lab:wall_8",30,true,-1,100.0f,"", true, 0);//CreateWallB8;
        

        chr.currentLvl.legend['Z'] = CreateObjectFunction("spritesheets:lab:wall_bottom_5_blank",30,false,-1,100.0f,"",true,0); //BlankWall
        chr.currentLvl.legend['T'] = CreateObjectFunction("spritesheets:lab:tank_classic",20,true,-1,100.0f,"",false,0);//Tank
        chr.currentLvl.legend['R'] = CreateObjectFunction("spritesheets:lab:tank_broken",20,true,-1,100.0f,"",false,0);//TankBroken
        chr.currentLvl.legend['U'] = CreateObjectFunction("spritesheets:lab:tank_classic",15,true,-1,107.1f,"lab:tank",false,0);//TankBubbling
        chr.currentLvl.legend['K'] = CreateObjectFunction("spritesheets:lab:acid_tenk",15,true,-1,107.1f,"lab:tank",false,0); //Acid tank
        chr.currentLvl.legend['Y'] = CreateObjectFunction("spritesheets:lab:acid_tenk_spill",15,true,-1,107.1f,"lab:tank",false,0); //Acid tank spill


        //enemy
        chr.currentLvl.legend['H'] = CreateObjectFunction("Skeleton",20,true,-1,100.0f,"skeleton:run",false,1);//enemy horizontal
        chr.currentLvl.legend['V'] = CreateObjectFunction("Skeleton", 20, true, -1, 100.0f, "skeleton:run",false, 2);//enemy vertical
        chr.currentLvl.legend['L'] = CreateObjectFunction("Skeleton", 20, true, -1, 100.0f, "skeleton:run",false, 3);//enemy follow

        //Room Transition
        chr.currentLvl.legend['8'] = CreateObjectFunction("spritesheets:lab:floor_1",30,true,1,100.0f,"",false,0);//Hall
        chr.currentLvl.legend['9'] = CreateObjectFunction("spritesheets:lab:floor_1",30,true,0,100.0f,"",false,0);//MainRoom
        chr.currentLvl.legend['F'] = CreateObjectFunction("lab:door:lab anims_Animation 5_00",30,true,3,107.1f,"lab:door",false,0);//Door, Room3
        chr.currentLvl.legend['E'] = CreateObjectFunction("spritesheets:lab:floor_1",30,true,2,100.0f,"",false,0);//Room2
        chr.currentLvl.legend['B'] = CreateObjectFunction("spritesheets:lab:floor_2",30,true,4,100.0f,"",false,0);//Room4
        chr.currentLvl.legend['C'] = CreateObjectFunction("spritesheets:lab:floor_3",30,true,5,100.0f,"",false,0);//Room5
        chr.currentLvl.legend['D'] = CreateObjectFunction("spritesheets:lab:floor_4",30,true,6,100.0f,"",false,0);//Hallway up
        chr.currentLvl.legend['X'] = CreateObjectFunction("lab:door:lab anims_Animation 5_00",30,true,7,107.1f,"lab:door",false,0);//Door, Boss


        //sprite.color = ColorRGBA(1, 0.5f, 1, 1);
        TilemapLegend legend=chr.currentLvl.legend;
        Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/lab/lab.map", &legend });
        Engine::Dispatcher().trigger<TilemapLoadRequest>(TilemapLoadRequest{ "tilemaps/lab/addons_lab.map", &legend });

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
/*
      
    for (int i = 0; i < 3; i++)
    {
        auto entity = reg.create();
        auto& bandit = reg.emplace<Bandit>(entity);
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Bandit");
        sprite.scale = { 1, 1 };
        if (i == 0)
        {
            bandit.ID = horizontal;
            sprite.position = { 50, 0, 0.0f };
        }

        if (i == 1)
        {
            bandit.ID = vertical;
            sprite.position = { 100, 30, 0.0f };
        }

        if (i == 2)
        {
            bandit.ID = follower;
            sprite.position = { 100, -30, 0.0f };
        }

        auto& transform = reg.emplace<Transform>(entity);
        transform.position = sprite.position;

        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size = {sprite.size.x / 5, sprite.size.y /2 };
    }

*/
}

