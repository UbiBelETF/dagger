#include "racing_game_logic.h"

#include "core/engine.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/racing/racing_player_car.h"
#include "gameplay/racing/racing_main.h"
#include "core/graphics/sprite.h"

using namespace dagger;
using namespace racing_game;

void RacingCollisionsLogicSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&RacingCollisionsLogicSystem::OnEndOfFrame>(this);
}

void RacingCollisionsLogicSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&RacingCollisionsLogicSystem::OnEndOfFrame>(this);
}

void racing_game::CreateStrike(float tileSize_,int life_)
{
    auto& reg = Engine::Instance().Registry();
 
     for(int i=0; i<life_;i++)
     {
        auto entity = reg.create();
        auto& sprite = reg.emplace<Sprite>(entity);
        AssignSprite(sprite, "Racing:police-car-bmw-z4"); 
        sprite.size = Vector2(1, 2) * tileSize_;

        sprite.color = ColorRGBA(1, 0.5f, 1, 1);
        auto& transform = reg.emplace<Transform>(entity);
        transform.position={ (11.7+i)*tileSize_,12*tileSize_, 1 };
    } 
}

void RacingCollisionsLogicSystem::Run()
{
    RacingGameFieldSettings fieldSettings;
    {
        static Entity entityFieldSettings;
        if (auto* ptr = Engine::GetDefaultResource<RacingGameFieldSettings>())
        {
            fieldSettings = *ptr;
        }

        auto view = Engine::Registry().view<RacingPlayerCar, Transform, SimpleCollision>();
        for (auto entity : view)
        {
            auto &t = view.get<Transform>(entity);
            auto &player = view.get<RacingPlayerCar>(entity);
            auto &col = view.get<SimpleCollision>(entity);

            if (col.colided)
            {
                col.colided=false;
                if(player.pause>0) player.pause--;
                else
                {    
                     
                    player.pause=200;
                    player.strike++;
                    racing_game::CreateStrike(20.f,player.strike);
                    
                    if(player.strike==3)
                    {
                        m_Restart = true;
                    }
                           
                }
                
                
            }
        }
    }
}

void RacingCollisionsLogicSystem::OnEndOfFrame()
{
    if (m_Restart)
    {
        m_Restart = false;
         Engine::Registry().clear();
        racing_game::SetupWorld(Engine::Instance());
        
    }
}