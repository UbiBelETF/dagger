#include "plight_game_logic.h"

#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/animation.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "core/graphics/text.h"

#include "gameplay/plight/plight.h"
#include "gameplay/plight/plight_controller.h"
#include "gameplay/plight/plight_combat.h"
#include "gameplay/plight/plight_aiming.h"
#include "gameplay/plight/plight_collisions.h"
#include "gameplay/plight/plight_projectiles.h"

using namespace dagger;
using namespace plight;

void PlightGameLogicSystem::SpinUp()
{
    Engine::Dispatcher().sink<NextFrame>().connect<&PlightGameLogicSystem::OnEndOfFrame>(this);
}

void PlightGameLogicSystem::WindDown()
{
    Engine::Dispatcher().sink<NextFrame>().disconnect<&PlightGameLogicSystem::OnEndOfFrame>(this);
}

void PlightGameLogicSystem::Run()
{

        auto view1 = Engine::Registry().view<PlightGameInfo>();
        for (auto entity : view1) {
            auto& gameInfo = view1.get<PlightGameInfo>(entity);

            auto view = Engine::Registry().view<PlightCharacterController>();
            for (auto entity : view) {
                auto& character = view.get<PlightCharacterController>(entity);
                if (character.dead && !gameInfo.displayingMessageEndGame) {
                    gameInfo.endGame = true;
                    if (character.playerNumber == "Player 1") {
                        gameInfo.endGameMessageString3 = "Player 2";
                    }
                    else {
                        gameInfo.endGameMessageString3 = "Player 1";
                    }
                    
                    break;
                }
            }

            if (gameInfo.newGame) {
                gameInfo.newGameMessage = Engine::Registry().create();
                auto& text = Engine::Registry().emplace<Text>(gameInfo.newGameMessage);
                text.Set("pixel-font", gameInfo.newGameMessageString1, {50.f,150.f,0.f});
                gameInfo.newGame = false;
                gameInfo.displayingMessage = true;
            }
            if (gameInfo.displayingMessage) {
                gameInfo.currentMessageDuration += Engine::DeltaTime();
               
                 if (gameInfo.currentMessageDuration >= gameInfo.newGameMessageDuration) {
                    gameInfo.displayingMessage = false;
                    gameInfo.displayingMessage2 = false;
                    gameInfo.currentMessageDuration = 0.f;
                    auto& text = Engine::Registry().get<Text>(gameInfo.newGameMessage);
                    for (auto ent : text.entities) {
                        Engine::Registry().remove_all(ent);
                    }
                    Engine::Registry().remove_all(gameInfo.newGameMessage);
                }
                 else if (gameInfo.currentMessageDuration >= gameInfo.newGameMessageDuration / 2 && !gameInfo.displayingMessage2) {
                     auto& text = Engine::Registry().get<Text>(gameInfo.newGameMessage);
                     text.Set("pixel-font", gameInfo.newGameMessageString2, { 50.f,150.f,0.f });
                     gameInfo.displayingMessage2 = true;
                 }
            }
            if (gameInfo.endGame) {
                gameInfo.endGameMessage1 = Engine::Registry().create();
                auto& text1 = Engine::Registry().emplace<Text>(gameInfo.endGameMessage1);
                text1.Set("pixel-font", gameInfo.endGameMessageString1, { 50.f,150.f,0.f });
                
                gameInfo.endGameMessage3 = Engine::Registry().create();
                auto& text3 = Engine::Registry().emplace<Text>(gameInfo.endGameMessage3);
                text3.Set("pixel-font", gameInfo.endGameMessageString3 + " Wins!", { 50.f,100.f,0.f });

                gameInfo.endGameMessage2 = Engine::Registry().create();
                auto& text2 = Engine::Registry().emplace<Text>(gameInfo.endGameMessage2);
                text2.Set("pixel-font", gameInfo.endGameMessageString2, { 50.f,50.f,0.f });
                gameInfo.endGame = false;
                gameInfo.displayingMessageEndGame = true;
            }

            
        }  
        auto view = Engine::Registry().view<InputReceiver>();
        for (auto entity : view)
        {
            auto& input = view.get<InputReceiver>(entity);
            if (EPSILON_NOT_ZERO(input.Get("restart"))) {
                m_Restart = true;
            }
        }
    
}

void PlightGameLogicSystem::OnEndOfFrame()
{
    if (m_Restart)
    {
        m_Restart = false;

        plight::ResetCharacters();

       auto view1 = Engine::Registry().view<PlightGameInfo>();
       for (auto entity : view1) {
           auto& gameInfo = view1.get<PlightGameInfo>(entity);
           if (gameInfo.displayingMessageEndGame) {
               auto& text1 = Engine::Registry().get<Text>(gameInfo.endGameMessage1);
               for (auto ent : text1.entities) {
                   Engine::Registry().remove_all(ent);
               }
               Engine::Registry().remove_all(gameInfo.endGameMessage1);

               auto& text2 = Engine::Registry().get<Text>(gameInfo.endGameMessage2);
               for (auto ent : text2.entities) {
                   Engine::Registry().remove_all(ent);
               }
               Engine::Registry().remove_all(gameInfo.endGameMessage2);

               auto& text3 = Engine::Registry().get<Text>(gameInfo.endGameMessage3);
               for (auto ent : text3.entities) {
                   Engine::Registry().remove_all(ent);
               }
               Engine::Registry().remove_all(gameInfo.endGameMessage3);
               gameInfo.displayingMessageEndGame = false;
               gameInfo.endGameMessageString3 = "";
           }
           if (gameInfo.displayingMessage) {
               auto& text = Engine::Registry().get<Text>(gameInfo.newGameMessage);
               for (auto ent : text.entities) {
                   Engine::Registry().remove_all(ent);
               }
               Engine::Registry().remove_all(gameInfo.newGameMessage);
               gameInfo.displayingMessage = false;
               gameInfo.displayingMessage2 = false;
               gameInfo.currentMessageDuration = 0.f;
           }
           gameInfo.newGame = true;
           
       }

       auto view2 = Engine::Registry().view<Projectile>();
       for (auto entity : view2) {
           Engine::Registry().remove_all(entity);
       }

      
      
    }
}