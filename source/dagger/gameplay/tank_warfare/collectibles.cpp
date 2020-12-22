#include "collectibles.h"

#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/graphics/animation.h"
#include "core/graphics/shaders.h"
#include "core/graphics/window.h"
#include "core/game/transforms.h"

#include "gameplay/common/simple_collisions.h"
#include "gameplay/tank_warfare/tank_stats.h"

using namespace tank_warfare;

int CollectibleSystem::s_NumCoins = 20;
int CollectibleSystem::s_NumPowers = 3;

void CollectibleSystem::Run()
{
	auto view = Engine::Registry().view<Transform, SimpleCollision, Collectible>();

	for (auto entity : view)
	{
		auto& col = view.get<SimpleCollision>(entity);
		auto& collectible = view.get<Collectible>(entity);

		if (col.colided)
		{
			if (Engine::Registry().has<TankCharacter>(col.colidedWith))
			{
				auto& tank = Engine::Registry().get<TankCharacter>(col.colidedWith);
				if (collectible.collType == ECollectibleType::Coin)
				{
					tank.coins++;
					s_NumCoins--;
				}
				else
				{
					if (collectible.collType == ECollectibleType::ExtraLife)
					{
						tank.lives++;
					}
					if (collectible.collType == ECollectibleType::FireRateUp)
					{
						tank.fireRate = tank.maxFireRate;
						tank.powerDuration += m_PowerDuration;
					}
					if (collectible.collType == ECollectibleType::Health)
					{
						tank.health = tank.maxHealth;
					}
					if (collectible.collType == ECollectibleType::Shield)
					{
						tank.shield = tank.maxShield;
					}
					s_NumPowers--;
				}
				collectible.toBeDestroyed = true;
			}
			col.colided = false;
		}
	}

	while (s_NumPowers < m_MaxPowers)
	{
		AddCollectible(true);
		s_NumPowers++;
	}

	while (s_NumCoins < m_MaxCoins)
	{
		AddCollectible(false);
		s_NumCoins++;
	}

}

void CollectibleSystem::SpinUp()
{
	Engine::Dispatcher().sink<NextFrame>().connect<&CollectibleSystem::OnEndOfFrame>(this);
}

void CollectibleSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&CollectibleSystem::OnEndOfFrame>(this);
}

void CollectibleSystem::OnEndOfFrame()
{
	auto view = Engine::Registry().view<Collectible>();

	for (auto entity : view)
	{
		auto& collectible = view.get<Collectible>(entity);
		if (collectible.toBeDestroyed)
		{
			Engine::Registry().destroy(entity);
		}
	}
}

void CollectibleSystem::ResetNumCoinsPowers()
{
	s_NumCoins = 0;
	s_NumPowers = 0;
}

void tank_warfare::AddCollectible(bool isPower_)
{
	auto& reg = Engine::Registry();
	auto entity = reg.create();
	auto& sprite = reg.emplace<Sprite>(entity);
	auto& anim = reg.emplace<Animator>(entity);

	if (isPower_) 
	{
		int collType = 1 + rand() % 4;
		AssignSprite(sprite, fmt::format("jovanovici:powerup:power{}", collType));
		AnimatorPlay(anim, fmt::format("powerup:power{}", collType));

		auto& collectible = reg.emplace<Collectible>(entity);
		collectible.collType = static_cast<ECollectibleType>(collType);
	}
	else
	{
		ECollectibleType collType = ECollectibleType::Coin;
		AssignSprite(sprite, "jovanovici:coin:coin1");
		AnimatorPlay(anim, "coin:spin");

		auto& collectible = reg.emplace<Collectible>(entity);
		collectible.collType = collType;
	}

	auto& transform = reg.emplace<Transform>(entity);
	int i = rand() % 20 - 10;
	int j = rand() % 20 - 10;
	transform.position = { 48 * i, 48 * j, 2 };
	
	auto& col = reg.emplace<SimpleCollision>(entity);
}
