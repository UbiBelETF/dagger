#include "gameplay/team_game/character_controller_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "gameplay/team_game/player_controller_fsm.h"
#include "gameplay/team_game/shoot.h"
#include "gameplay/team_game/team_game_main.h"

using namespace dagger;
using namespace lab;

// Idle

void CharacterFSM::Idle::Enter(CharacterFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "main_character:idle");
}

DEFAULT_EXIT(CharacterFSM, Idle);

void CharacterFSM::Idle::Run(CharacterFSM::StateComponent& state_) 
{   
    auto&& [sprite,transform, input, character] = Engine::Registry().get<Sprite,Transform, InputReceiver, lab::PlayerCharacter>(state_.entity);

    if (EPSILON_NOT_ZERO(input.Get("rightleft")) || EPSILON_NOT_ZERO(input.Get("updown")))
	{
		GoTo(ECharacterState::Running, state_);
	}
    if (EPSILON_NOT_ZERO(input.Get("shoot"))) 
    {
        if (character.cooldown <= 0)
        {
            auto cursorInWindow = dagger::Input::CursorPositionInWorld();
            Vector2 position = { transform.position.x, transform.position.y };
            Vector2 cursor = { cursorInWindow.x, cursorInWindow.y };
            lab::CreateBullet(position, cursor, player, "Bullet");
            character.cooldown = character.maxCooldown;
        }
    }
     character.cooldown--;
    if(EPSILON_NOT_ZERO(input.Get("restart")) )
    {
        Engine::Registry().clear();
        lab::SetupWorld(Engine::Instance());
    }

}


// Running

void CharacterFSM::Running::Enter(CharacterFSM::StateComponent& state_) 
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "main_character:run");
}


void CharacterFSM::Running::Exit(CharacterFSM::StateComponent& state_) 
{}

void CharacterFSM::Running::Run(CharacterFSM::StateComponent& state_) 
{
	auto&& [sprite,transform, input, character] = Engine::Registry().get<Sprite,Transform, InputReceiver, lab::PlayerCharacter>(state_.entity);

	Float32 rl = input.Get("rightleft");
    Float32 ud = input.Get("updown");
    Float32 shoot = input.Get("shoot");
    Float32 fast = input.Get("fast");

	if (EPSILON_ZERO(rl) && EPSILON_ZERO(ud))
	{
		GoTo(ECharacterState::Idle, state_);
	}
	else 
    {
        if(EPSILON_NOT_ZERO(fast))
        {
            character.speed=80;
        }
        else
        {
            character.speed=50;
        }
        if (rl != 0)
        { 
            sprite.scale.x = rl;
            transform.position.x += character.speed * sprite.scale.x * Engine::DeltaTime();
                
        } 
        if (ud != 0)
        { 
            sprite.scale.y = 1;
            transform.position.y += character.speed * ud * Engine::DeltaTime();
                
        }
    }
    if (EPSILON_NOT_ZERO(shoot)) 
    {
        if (character.cooldown <= 0)
        {
            auto cursorInWindow = dagger::Input::CursorPositionInWorld();
            Vector2 position = { transform.position.x, transform.position.y };
            Vector2 cursor = { cursorInWindow.x, cursorInWindow.y };
            lab::CreateBullet(position, cursor, player, "Bullet");
            character.cooldown = character.maxCooldown;
        }
    }
     character.cooldown--;  
    if(EPSILON_NOT_ZERO(input.Get("restart")) )
    {
        Engine::Registry().clear();
        lab::SetupWorld(Engine::Instance());
    }
}
            
//Dying
void CharacterFSM::Dying::Enter(CharacterFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "main_character:death");
}

void CharacterFSM::Dying::Exit(CharacterFSM::StateComponent& state_) 
{
}
void CharacterFSM::Dying::Run(CharacterFSM::StateComponent& state_) 
{
    auto& input= Engine::Registry().get<InputReceiver>(state_.entity);
    if(EPSILON_NOT_ZERO(input.Get("restart")) )
    {
        Engine::Registry().clear();
        lab::SetupWorld(Engine::Instance());
    }
}

//GetHit
void CharacterFSM::GetHit::Enter(CharacterFSM::StateComponent& state_)
{
    auto& animator = Engine::Registry().get<Animator>(state_.entity);
    AnimatorPlay(animator, "main_character:hit");	
}

void CharacterFSM::GetHit::Exit(CharacterFSM::StateComponent& state_) 
{
        
}
void CharacterFSM::GetHit::Run(CharacterFSM::StateComponent& state_) 
{
    auto&& [sprite,transform, input, character] = Engine::Registry().get<Sprite,Transform, InputReceiver, lab::PlayerCharacter>(state_.entity);
    
    if(character.hitCooldown==0)
    {
        character.hitCooldown=30;
        
        if (EPSILON_NOT_ZERO(input.Get("rightleft")) || EPSILON_NOT_ZERO(input.Get("updown")))
        {
            GoTo(ECharacterState::Running, state_);
        }
        else
        {
            GoTo(ECharacterState::Idle, state_);
        }

    }
    else
    {
        character.hitCooldown--;
    }

    if(EPSILON_NOT_ZERO(input.Get("restart")))
    {
        Engine::Registry().clear();
        lab::SetupWorld(Engine::Instance());
    }


       
    
}

