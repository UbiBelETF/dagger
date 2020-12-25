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
#include "gameplay/plight/plight_fields.h"

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
		auto viewLS = Engine::Registry().view<PlightIntro, InputReceiver>();
		for (auto entity : viewLS) 
		{
			auto& pintro = viewLS.get<PlightIntro>(entity);
			if (pintro.IsFinished()) 
			{
				break;
			}
			if (!pintro.displaying) {
				pintro.team = Engine::Registry().create();
				auto& teamTxt = Engine::Registry().emplace<Text>(pintro.team);
				teamTxt.Set("pixel-font", pintro.teamName, {0, 230 ,0 });

				pintro.team1 = Engine::Registry().create();
				auto& teamTxt1 = Engine::Registry().emplace<Text>(pintro.team1);
				teamTxt1.Set("pixel-font", pintro.teamName1, { 0, 180 ,0 });

				pintro.message1 = Engine::Registry().create();
				auto& msg1 = Engine::Registry().emplace<Text>(pintro.message1);
				msg1.Set("pixel-font", pintro.introText1, { 0,-150,0 });

				pintro.message = Engine::Registry().create();
				auto& msg = Engine::Registry().emplace<Text>(pintro.message);
				msg.Set("pixel-font", pintro.introText, { 0,-100,0 });

				pintro.displaying = true;
			}

			


			auto& input = viewLS.get<InputReceiver>(entity);
			Float32 procced = input.Get("intro");
			if (EPSILON_NOT_ZERO(procced))
			{
				pintro.IntroFinished();
			}

		}

		auto viewLS1 = Engine::Registry().view<PlightIntro>();
		auto it = viewLS1.begin();
		auto& pin = viewLS1.get<PlightIntro>(*it);

		if (pin.IsFinished()) {
			if (!pin.BattleOn) {
				auto& text4 = Engine::Registry().get<Text>(pin.team);
				for (auto ent : text4.entities) {
					Engine::Registry().remove_all(ent);
				}
				Engine::Registry().remove_all(pin.team);

				auto& text1 = Engine::Registry().get<Text>(pin.team1);
				for (auto ent : text1.entities) {
					Engine::Registry().remove_all(ent);
				}
				Engine::Registry().remove_all(pin.team1);

				auto& text2 = Engine::Registry().get<Text>(pin.message);
				for (auto ent : text2.entities) {
					Engine::Registry().remove_all(ent);
				}
				Engine::Registry().remove_all(pin.message);

				auto& text3 = Engine::Registry().get<Text>(pin.message1);
				for (auto ent : text3.entities) {
					Engine::Registry().remove_all(ent);
				}
				Engine::Registry().remove_all(pin.message1);
				pin.BattleOn = true;
			}
			


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
					text.Set("pixel-font", gameInfo.newGameMessageString1, { 50.f,150.f,0.f });
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
}

void PlightGameLogicSystem::OnEndOfFrame()
{
    if (m_Restart)
    {
        m_Restart = false;

        plight::ResetCharacters();

		auto view = Engine::Registry().view<DefenseField>();
		for (auto entity : view) {
			auto& defenseField = Engine::Registry().get<DefenseField>(entity);
			Engine::Registry().remove_all(defenseField.defenseFieldE);
		}
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
	auto viewLS1 = Engine::Registry().view<PlightIntro, Sprite>();
	auto it = viewLS1.begin();
	auto& pin = viewLS1.get<PlightIntro>(*it);

	if (pin.IsFinished() && pin.displaying )
	{
		pin.currDT += Engine::DeltaTime();
		if (pin.currDT >= pin.displayTime )
		{

		}
		else
		{
			auto& sprt = viewLS1.get<Sprite>(*it);
			if (1.f - pin.currDT > 0.f) {
				sprt.color = { 1.f - pin.currDT , 1.f - pin.currDT, 1.f - pin.currDT , 1.f - pin.currDT };
			}
			else {
				sprt.color = { 0.f , 0.f, 0.f,0.f };
			}
			
		}

	}

}