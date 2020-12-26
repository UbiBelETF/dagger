#include "attack_system.h"

void team_game::AttackSystem::Run()
{
	auto viewAttacks = Engine::Registry().view<Attack,Transform>();
	for (auto attackPnt : viewAttacks) {
		auto& transform = viewAttacks.get<Transform>(attackPnt);

		auto& attack = viewAttacks.get<Attack>(attackPnt);
		auto&& [transformOrig, offsetOrig] = Engine::Registry().get<Transform, AttackOffset>(attack.orig);
		transform.position = transformOrig.position+offsetOrig.offsetVec;
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
