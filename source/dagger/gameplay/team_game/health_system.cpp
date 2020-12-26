#include "health_system.h"
#include "hero_controller.h"
#include "slime_controller.h"
#include "tilemap_legends.h"
#include "core/graphics/text.h"
#include "core/game/transforms.h"
#include "core/graphics/sprite.h"
#include <string>
#include "collision.h"
namespace team_game {
	
	void HealthSystem::Run()
	{
		auto viewSlimes = Engine::Registry().view<Health, Transform, TeamGameSlime>();
		auto viewHero = Engine::Registry().view<Health, Transform, TeamGameCharacter>();
		auto viewPots = Engine::Registry().view<Health, TeamGamePot>();
		auto viewCrates = Engine::Registry().view<Health, TeamGameCrate>();
		auto hero = viewHero.begin();
		auto& heroTransform = viewHero.get<Transform>(*hero);
		auto& heroHealth = viewHero.get<Health>(*hero);
		if (heroHealth.hp < 0) {
			heroHealth.hp = 0;
		}
		if (heroHealth.show) {
			auto& hpBarSprite = Engine::Registry().get<Sprite>(heroHealth.hpBar);
			auto& hpBarTransform = Engine::Registry().get<Transform>(heroHealth.hpBar);
			float hp = heroHealth.hp*1.0f / heroHealth.maxHp;
			hpBarSprite.scale = Vector2(hp, 1);
			hpBarTransform.position = heroTransform.position + Vector3(0, -10, 0);
			int a=3;
			a++;
		}
		for (auto slime : viewSlimes) {
			auto& slimeTransform = viewSlimes.get<Transform>(slime);
			auto& slimeSprite = viewSlimes.get<Transform>(slime);
			auto& slimeHealth = viewSlimes.get<Health>(slime);
			if (slimeHealth.hp < 0) {
				slimeHealth.hp = 0;
				auto& slimeSprite = Engine::Registry().get<Sprite>(slime);
				auto& slimeAi = Engine::Registry().get<SlimeAi>(slime);
				slimeAi.alive = false;
				AssignSprite(slimeSprite, "spritesheets:tiles_dungeon:empty");
				auto& slimeCol = Engine::Registry().get<Collision>(slime);
				slimeCol.size = Vector2{ 0,0 };
			
			}
			if (slimeHealth.show) {
				auto& hpBarSprite = Engine::Registry().get<Sprite>(slimeHealth.hpBar);
				auto& hpBarTransform = Engine::Registry().get<Transform>(slimeHealth.hpBar);
				float hp = slimeHealth.hp * 1.0f / slimeHealth.maxHp;
				hpBarSprite.scale = Vector2(hp, 1);
				hpBarTransform.position = slimeTransform.position+Vector3(0,-10,0);
			}
		}
		for (auto pot : viewPots) {
		
			auto& potHealth = viewPots.get<Health>(pot);
			if (potHealth.hp <= 0) {
				potHealth.hp = 0;
				auto& potSprite = Engine::Registry().get<Sprite>(pot);
				auto& potCol = Engine::Registry().get<Collision>(pot);
				potCol.size = Vector2{ 0,0 };
				AssignSprite(potSprite, "spritesheets:tiles_dungeon:pot_damaged");

				//Engine::Registry().remove<Collision>(pot);
				//Engine::Registry().remove<Health>(pot);
			}

		}
		for (auto crate : viewCrates) {

			auto& crateHealth = viewCrates.get<Health>(crate);
			if (crateHealth.hp <= 0) {
				crateHealth.hp = 0;
				auto& crateSprite = Engine::Registry().get<Sprite>(crate);
				auto& crateCol = Engine::Registry().get<Collision>(crate);
				crateCol.size = Vector2{ 0,0 };
				AssignSprite(crateSprite, "spritesheets:tiles_dungeon:crate_damaged");
				//Engine::Registry().remove<Collision>(crate);
				//Engine::Registry().remove<Health>(crate);
			}

		}
	}
}