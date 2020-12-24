#define _USE_MATH_DEFINES


#include "plight_melee.h"


#include "core/engine.h"
#include "core/graphics/sprite.h"
#include "core/input/inputs.h"
#include "core/game/transforms.h"
#include <cmath>
#include <math.h>

#include "gameplay/plight/plight_collisions.h"
#include "gameplay/plight/plight_combat.h"
#include "gameplay/plight/plight_controller.h"
#include "gameplay/plight/plight_aiming.h"
#include "gameplay/plight/plight_particles.h"
#include "gameplay/plight/plight_physics.h"
#include "gameplay/plight/plight_game_logic.h"

#include <algorithm>
#include <execution>


using namespace dagger;
using namespace plight;

void MeleeSystem::Run() 
{
	auto viewLS1 = Engine::Registry().view<PlightIntro>();
	auto it = viewLS1.begin();
	auto& pin = viewLS1.get<PlightIntro>(*it);

	if (pin.IsFinished())
	{
		auto ChrView = Engine::Registry().view<Transform, InputReceiver, CombatStats, PlightCharacterController>();
		for (auto& entity : ChrView) {
			auto& chr = ChrView.get<PlightCharacterController>(entity);
			auto& com = ChrView.get<CombatStats>(entity);
			auto& input = ChrView.get<InputReceiver>(entity);
			if (chr.dead) {
				continue;
			}

			Float32 attack = input.Get("attack");
			if (EPSILON_NOT_ZERO(attack)) {
				if (chr.attacking) {
					//Pusti se animacija
				}
				else {
					auto weapon_sprt = Engine::Registry().get<Sprite>(chr.weaponSprite);
					AssignSprite(weapon_sprt, chr.meleeWeaponSpriteName);

				}

			}
			
		}


	}
}

void MeleeSystem::SpinUp()
{
	Engine::Dispatcher().sink<NextFrame>().connect<&MeleeSystem::OnEndOfFrame>(this);

}

void MeleeSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&MeleeSystem::OnEndOfFrame>(this);
}


void MeleeSystem::OnEndOfFrame() {

}