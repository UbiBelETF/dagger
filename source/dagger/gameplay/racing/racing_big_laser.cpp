#include "racing_big_laser.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "racing_player_car.h"

#include "gameplay/racing/racing_simple_collision.h"


#include "gameplay/racing/racing_game_logic.h"

using namespace dagger;
using namespace racing_game;

void BigLaserSystem::Run()
{
	RacingGameFieldSettings fieldSettings;
	if (auto* ptr = Engine::GetDefaultResource<RacingGameFieldSettings>())
	{
	    fieldSettings = *ptr;
	}

	auto view = Engine::Registry().view<BigLaserGun, Transform, ControllerMapping, Sprite, RacingSimpleCollision>();
	for (auto entity : view)
	{
		auto& t = view.get<Transform>(entity);
		auto& ctrl = view.get<ControllerMapping>(entity);
		auto& laser = view.get<BigLaserGun>(entity);
		auto& col = view.get<RacingSimpleCollision>(entity);
		
		t.position.x += ctrl.input.x * laser.laserSpeed * Engine::DeltaTime();
		
		Float32 boarderX = fieldSettings.GetXBoarder();
		if (t.position.x > boarderX)
		{
		    t.position.x = boarderX;
		}
		
		if (t.position.x < -boarderX)
		{
		    t.position.x = -boarderX;
		}

		float TileSize = fieldSettings.fieldTileSize;

		if (ctrl.inputFireLaser == 1)//laser ready to be fired
		{
			ctrl.inputFireLaser = -2;
			laser.remainingCooldown = 2;

			auto& sprite = view.get<Sprite>(entity);
			AssignSprite(sprite, "Racing:laser-long");
			float ratio = sprite.size.y / sprite.size.x;
			sprite.size = { 2 * TileSize, 5 * TileSize * ratio };

			col.isColisionOn = true;
			col.identifier = 2;
		}
		else if (ctrl.inputFireLaser == -2 && laser.remainingCooldown <= 1.5f)//laser had been firing for 0.5 seconds
		{
			ctrl.inputFireLaser = -1;
			laser.remainingCooldown -= Engine::DeltaTime();

			auto& sprite = view.get<Sprite>(entity);
			AssignSprite(sprite, "Racing:laser-long-off");
			float ratio = sprite.size.y / sprite.size.x;
			sprite.size = { 2 * TileSize, 5 * TileSize * ratio };

			col.isColisionOn = false;
		}
		else if (ctrl.inputFireLaser <= -1 && laser.remainingCooldown > 0)//laser has finished firing but is still on cooldown
		{
			laser.remainingCooldown -= Engine::DeltaTime();
		}
		else if (ctrl.inputFireLaser <= -1 && laser.remainingCooldown <= 0)//laser is no longer on cooldown and is being put in wait mode again
		{
			ctrl.inputFireLaser = 0;
			laser.remainingCooldown = 0;
		}
		
		if (ctrl.inputFireLaser == -2 && col.colided)//if the laser hit something while firing remove the collision, change the sprite of the object, and update the score
		{
			col.colided = false;
			Engine::Registry().remove<RacingSimpleCollision>(col.colidedWith);
			auto& sprite = Engine::Registry().get<Sprite>(col.colidedWith);
			AssignSprite(sprite, "Racing:police-car-bmw-z4-exploded");
			float ratio = sprite.size.y / sprite.size.x;
			sprite.size = { 2 * TileSize, 2 * TileSize * ratio };
			sprite.scale.y = -1;

			auto& view = Engine::Registry().view<RacingGameStats>();
			for (auto entity : view)
			{
				auto& gameStats = Engine::Registry().get<RacingGameStats>(entity);
				gameStats.scores -= 1;
			}
		}


	}
}
