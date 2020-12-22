#pragma once

#include "core/system.h"
#include "core/core.h"

using namespace dagger;

namespace plight
{
	struct PlightIntro {
		
		bool loading_screen = true;
		String introText = "Welcome  Warriors!";
		String introText1 = "PRESS 'ENTER' TO BATTLE";
		String teamName = "Presented by:";
		String teamName1 = " Team  MorningAfternoon";

		bool displaying = false;
		bool BattleOn = false;
		
		Float32 displayTime = 2.f;
		Float32 currDT = 0.f;

		Entity team;
		Entity message;
		Entity team1;
		Entity message1;
		

		bool IsFinished() {
			return (!loading_screen);
		}
		bool IntroFinished() {
			loading_screen = false;
		}
	};

    struct PlightGameInfo {
        bool newGame = true;
        bool displayingMessage = false;
        bool displayingMessage2 = false;

        bool displayingMessageEndGame = false;


        String newGameMessageString1 = "New Game!";
        String newGameMessageString2 = "Battle!";

        bool endGame = false;
        String endGameMessageString1 = "Game Over!";
        String endGameMessageString2 = "Press Y For A New Game";
        String endGameMessageString3 = "";

        Entity newGameMessage;
        Entity endGameMessage1;
        Entity endGameMessage2;
        Entity endGameMessage3;

        Float32 newGameMessageDuration = 2.f;
        Float32 currentMessageDuration = 0.f;

    };
   
    class PlightGameLogicSystem
        : public System
    {
        bool m_Restart = false;
       

    public:
        inline String SystemName() { return "Plight Game Logic System System"; }

        void SpinUp() override;
        void WindDown() override;
        void Run() override;

    private:
        void OnEndOfFrame();

    };
}
