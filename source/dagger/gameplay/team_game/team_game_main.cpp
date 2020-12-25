#include "team_game_main.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"
#include "core/graphics/text.h"

#include "gameplay/team_game/death_counter.h"
#include "gameplay/team_game/brawler_controller.h"
#include "gameplay/team_game/game_manager.h"
#include "gameplay/team_game/player_camera_focus.h"
#include "gameplay/common/simple_collisions.h"
#include "gameplay/team_game/physics.h"
#include "gameplay/team_game/collisions.h"
#include "gameplay/team_game/boss_fsm.h"
#include "gameplay/team_game/combat.h"

using namespace dagger;
using namespace team_game;

void TeamGame::GameplaySystemsSetup(Engine &engine_)
{
    engine_.AddPausableSystem<CameraFollowSystem>();
    engine_.AddPausableSystem<PhysicsSystem>();
    engine_.AddPausableSystem<GameManagerSystem>();
    engine_.AddPausableSystem<BrawlerControllerSystem>();
    //engine_.AddPausableSystem<SimpleCollisionsSystem>();
    engine_.AddPausableSystem<CombatSystem>();
    engine_.AddPausableSystem<CollisionSystem>();
    engine_.AddPausableSystem<DeathsCounterSystem>();
    engine_.AddPausableSystem<TransformSystem>();
}


void SetupCamera()
{
    auto* camera = Engine::GetDefaultResource<Camera>();
    camera->mode = ECameraMode::FixedResolution;
    camera->size = { 800, 600 };
    camera->zoom = 2;
    camera->position = { 0, 0, 0 };
    camera->Update();
}





//Player
struct Player
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    BrawlerCharacter& character;
    Transform& transform;
    Physics& physics;
    SimpleCollision& col;


    static Player Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<BrawlerCharacter>(entity);
        auto& transform = reg.get_or_emplace<Transform>(entity);
        auto& physics = reg.get_or_emplace<Physics>(entity);
        auto& col=reg.get_or_emplace<SimpleCollision>(entity);
        ATTACH_TO_FSM(ControllerFSM, entity);
        ATTACH_TO_FSM(AnimationsFSM, entity);
        character.attackSize = 25;
        col.size.x = 5;
        col.size.y = 15;
        physics.nonStatic = true;
        character.player = true;
        return Player{ entity, sprite, anim, input, character,transform,physics,col };
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
        chr.sprite.position = { position_, 1.0f };
        chr.sprite.color = { color_, 1.0f };
        
        //chr.transform.position = {2160, 1175, 0.0f };
        //chr.transform.position = { 2550, 26, 0.0f };
        chr.transform.position = { position_, 0.0f };

        AssignSprite(chr.sprite, "Light:idle");
        AnimatorPlay(chr.animator, "character:IDLE");


        if (input_ != "")
            chr.input.contexts.push_back(input_);

        chr.character.speed.x = 100;
        chr.character.speed.y = 250;

        return chr;
    }
};



//BOSS
struct Boss
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;
    InputReceiver& input;
    BrawlerCharacter& character;
    Transform& transform;
    Physics& physics;
    SimpleCollision& col;


    static Boss Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        auto& input = reg.get_or_emplace<InputReceiver>(entity);
        auto& character = reg.get_or_emplace<BrawlerCharacter>(entity);
        auto& transform = reg.get_or_emplace<Transform>(entity);
        auto& physics = reg.get_or_emplace<Physics>(entity);
        auto& col = reg.get_or_emplace<SimpleCollision>(entity);
        ATTACH_TO_FSM(BossFSM, entity);
        character.attackSize = 120;
        col.size.x = 20;
        col.size.y = 21;
        col.pivot = { -0.5f, -1.85f };
        sprite.pivot = { 0.2f, 0.0f };
        physics.nonStatic = true;
        return Boss{ entity, sprite, anim, input, character,transform,physics,col };
    }

    static Boss Create(
        String input_ = "",
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 100, 100 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto chr = Boss::Get(entity);

        chr.sprite.scale = { 1, 1 };
        chr.sprite.position = { position_, 1.0f };
        chr.sprite.color = { color_, 1.0f };

        chr.transform.position = { position_ ,0.f};

        AssignSprite(chr.sprite, "spritesheets:team_game:boss:idle:boss_idle");
        AnimatorPlay(chr.animator, "boss:boss_idle");


        if (input_ != "")
            chr.input.contexts.push_back(input_);

        chr.character.speed.x = 100;
        chr.character.speed.y = 250;

        return chr;
    }
};

//Guide
struct Guide
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;

    static Guide Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);          
        return Guide{ entity, sprite, anim};
    }

    static Guide Create(
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 100, 100 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto chr = Guide::Get(entity);
        chr.sprite.scale = { 1, 1 };
        chr.sprite.position = { 2430.0, 22, 0.0f };
        chr.sprite.color = { color_, 1.0f };
        AssignSprite(chr.sprite, "team_game:Guide:Animation0");
        AnimatorPlay(chr.animator, "guide:idle");
        return chr;
    }
};

//Bubble
struct Bubble
{
    Entity entity;
    Sprite& sprite;
    Animator& animator;

    static Bubble Get(Entity entity)
    {
        auto& reg = Engine::Registry();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);
        auto& anim = reg.get_or_emplace<Animator>(entity);
        return Bubble{ entity, sprite, anim };
    }

    static Bubble Create(
        ColorRGB color_ = { 1, 1, 1 },
        Vector2 position_ = { 100, 100 })
    {
        auto& reg = Engine::Registry();
        auto entity = reg.create();
        auto chr = Bubble::Get(entity);
        chr.sprite.scale = { 0.8, 0.8 };
        chr.sprite.position = { 2370, 67, 0.0f };
        chr.sprite.color = { color_, 1.0f };
        AssignSprite(chr.sprite, "team_game:Quotes:bubble1");
        AnimatorPlay(chr.animator, "quotes:talk");
        return chr;
    }
};
//bacckground
void CreateBackground()
{
   /* auto& reg = Engine::Registry();
    auto* camera = Engine::GetDefaultResource<Camera>();
    
    //Create terrain 
    {
        auto back = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(back);
        AssignSprite(sprite, "EmptyWhitePixel");
        sprite.color = { 0, 0, 0, 1 };
        sprite.size = { 200, 200 };
        sprite.scale = { 10, 1 };
        sprite.position = { 0, -125, 1 };
        
       
    }

    // down(ground) platfrom
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x =  800;
        col.size.y = 200;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 0;
        transform.position.y = -125;
        transform.position.z = 1;
    }

    // left wall
    {
        auto back = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(back);
        AssignSprite(sprite, "EmptyWhitePixel");
        sprite.color = { 0, 0, 0, 1 };
        sprite.size = { 100, 200 };
        sprite.scale = { 1, 1 };
        sprite.position = { -200, -75, 1 };
        auto& col = reg.emplace<SimpleCollision>(back);
        col.size.x = 100;
        col.size.y = 200;

        auto& transform = reg.emplace<Transform>(back);
        transform.position.x =-200;
        transform.position.y = -75;
        transform.position.z = 1;
    }

    //platform
    {
        auto back = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(back);
        AssignSprite(sprite, "EmptyWhitePixel");
        sprite.color = { 0, 0, 0, 1 };
        sprite.size = { 50, 20 };
        sprite.scale = { 1, 1 };
        sprite.position = { 200, 25, 1 };
        auto& col = reg.emplace<SimpleCollision>(back);
        col.size.x = 50;
        col.size.y = 20;
        auto& transform = reg.emplace<Transform>(back);
        transform.position.x = 200;
        transform.position.y = 25;
        transform.position.z = 1;
    }

    // Put background image 
    {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "souls_like_knight_character:BACKGROUND:Background");
        sprite.position.z = 10;
    }
    
    // Put grass  {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "souls_like_knight_character:BACKGROUND:Grass");
        sprite.position = { 0, -25, 5 };
    }

    // Put trees 
    {
        auto entity = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(entity);

        AssignSprite(sprite, "souls_like_knight_character:BACKGROUND:Tree");
        sprite.position = { 0, 30, 7 };
    }*/
    auto& reg = Engine::Registry();
    auto* camera = Engine::GetDefaultResource<Camera>();
    {
        auto entity = reg.create();
        auto& col = reg.emplace<SimpleCollision>(entity);
        col.size.x = 3000;
        col.size.y = 200;

        auto& transform = reg.emplace<Transform>(entity);
        transform.position.x = 1250;
        transform.position.y =-400;
        transform.position.z = 1;
    }
    
}

void team_game::SetupWorld(Engine& engine_)
{
    SetupCamera();

    auto mainChar = Player::Create("CONTROLS", { 1, 1, 1 }, { 0, 0 });
    Engine::Registry().emplace<CameraFollow>(mainChar.entity);



}

void team_game::SetupWorld_Demo(Engine& engine_)
{
    SetupCamera();
    auto& reg = Engine::Registry();
    auto* camera = Engine::GetDefaultResource<Camera>();
   
    
    auto mainChar = Player::Create("CONTROLS", { 1, 1, 1 }, { 0,100});
    auto boss = Boss::Create("Arrows", { 1,1,1 }, { 2630, 1100 });
    auto guide = Guide::Create( { 1,1,1 }, { 50,100 });
    auto bubble = Bubble::Create({ 1,1,1 }, { 50,100 });
    Engine::Registry().emplace<CameraFollow>(mainChar.entity);
    {
        auto deathsLogo = reg.create();
        auto& sprite = reg.get_or_emplace<Sprite>(deathsLogo);
        AssignSprite(sprite, "team_game:death_logo");
        sprite.color = { 1, 1, 1, 1.0f };
        sprite.scale = { 1, 1 };
        sprite.position = { -375, 265, 0 };
        sprite.UseAsUI();
        
        auto ui = reg.create();
        auto& text = reg.emplace<Text>(ui);
        auto& ct = reg.emplace<CounterType>(ui);
        text.spacing = 0.6f;
        text.Set("pixel-font", fmt::format("x {}", mainChar.character.deaths), { -300, 262,0 });
        //Engine::Registry().emplace<DeathCounter>(ui);
        {
            auto lives = reg.create();
            auto& sprite = reg.get_or_emplace<Sprite>(lives);
            auto& anim = reg.get_or_emplace<Animator>(lives);
            AssignSprite(sprite, "spritesheets:team_game:lives:heart_idle");
            AnimatorPlay(anim, "lives:heart_idle");
            sprite.color = { 1, 1, 1, 1.0f };
            sprite.scale = { 3, 3 };
            sprite.position = { -385, 225, 0 };
            sprite.UseAsUI();
            auto uii = reg.create();
            auto& text = reg.emplace<Text>(uii);
            auto& ct = reg.emplace<CounterType>(uii);
            ct.type = true;
            text.spacing = 0.6f;
            text.Set("pixel-font", fmt::format("x {}", mainChar.character.deaths), { -300, 222,0 });
        }
        {
            auto bar = reg.create();
            auto& sprite = reg.get_or_emplace<Sprite>(bar);
            auto& b = reg.get_or_emplace<BarOrCredits>(bar);
            AssignSprite(sprite, "team_game:Healthbar:barnone");
            sprite.color = { 1, 1, 1, 1.0f };
            sprite.scale = { 1, 1 };
            sprite.position = { 0, 265, 0 };
            sprite.UseAsUI();
        }
    }
}
void TeamGame::WorldSetup(Engine& engine_)
{
    SetupWorld_Demo(engine_);
}