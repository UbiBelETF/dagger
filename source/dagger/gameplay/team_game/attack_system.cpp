#include "attack_system.h"

void team_game::AttackSystem::Run()
{
	auto viewAttacks = Engine::Registry().view<Attack,Transform>();
	for (auto attackPnt : viewAttacks) {
		auto& transform = viewAttacks.get<Transform>(attackPnt);

		auto& attack = viewAttacks.get<Attack>(attackPnt);
		auto& transformAttack = Engine::Registry().get<Transform>(attack.attackEnt);
		transformAttack.position = transform.position + attack.offsetVec;
		if (attack.finished) {
			if (attack.damaged.size() > 0) {
				for (auto& damaged : attack.damaged) {
					damaged.hp -= attack.damage;
				}
				attack.damaged.clear();
			}
		}
	}
}
