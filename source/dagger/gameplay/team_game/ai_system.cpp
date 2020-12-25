#include "ai_system.h"
#include "slime_controller.h"
#include "core/game/transforms.h"
#include "gameplay/team_game/team_game_main.h"
#include "gameplay/team_game/hero_controller.h"
#include "core/graphics/sprite.h"
using namespace team_game;




void team_game::AiSystem::SpinUp()
{
}

void team_game::AiSystem::Run()
{
	counter += Engine::DeltaTime();
	if (counter >= delay) {
		counter = 0.0f;
			auto viewSlimes = Engine::Registry().view<SlimeAi,Transform,Sprite>();
			auto viewHero = Engine::Registry().view<TeamGameCharacter, Transform>();
			auto hero = viewHero.begin();
			auto& heroTransform = viewHero.get<Transform>(*hero);
			
			for( auto slime :viewSlimes)
			{
				auto& slimeAi = viewSlimes.get<SlimeAi>(slime);
				auto& slimeTransform = viewSlimes.get<Transform>(slime);
				auto& slimeSprite = viewSlimes.get<Sprite>(slime);
				Vector2 distanceVec = Vector2(slimeTransform.position.x - heroTransform.position.x, slimeTransform.position.y - heroTransform.position.y);
				if (slimeAi.previous.size() > depth)slimeAi.previous.pop_front();
				slimeAi.previous.push_back(slimeAi.current);
				SlimeAiCommand newCommand;
				newCommand.attack = false;
				float distance= glm::length(distanceVec);
				if (distance <= slimeSprite.size.x * 1.5) {
					if (distance<=slimeSprite.size.x*0.55) {
						newCommand.attack = true;
					}
					
					if (abs(distanceVec.x) > abs(distanceVec.y)) {
						if (distanceVec.x > 0)newCommand.move = LEFT;
						else newCommand.move = RIGHT;
					}
					else {
						if (distanceVec.y > 0)newCommand.move = UP;
						else newCommand.move = DOWN;

					}
				}
			else {
					if (rand() % 10 < 3) {
						
						newCommand.move = Movement((rand() % 4) );
						
					}
					else {
						INT16 previous[5] = { 0,0,0,0,0 };
						for (size_t i = 0; i < slimeAi.previous.size(); i++)previous[(int)slimeAi.previous.at(i).move]++;
						INT16 moveMax = 0;
						INT16 maxMoved = previous[0];
						for (INT16 i = 0; i < 5; i++) {
							if (maxMoved < previous[i]) {
								maxMoved = previous[i];
							    moveMax = i;
							}
						}
						newCommand.move = Movement(moveMax);
						
					}
				}
				slimeAi.current = newCommand;
			
			}
			
	}
	
}

void team_game::AiSystem::WindDown()
{
}



