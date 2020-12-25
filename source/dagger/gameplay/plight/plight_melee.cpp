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
#include "core/graphics/animations.h"

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

			
			auto& weapon = Engine::Registry().get<Weapon>(chr.weaponSprite);
			auto& weapon_sprite = Engine::Registry().get<Sprite>(chr.weaponSprite);
			if (weapon.animPlaying) {
				com.currentStamina -= STAMINA_FOR_ATTACKING * Engine::DeltaTime();
				auto& sprite = Engine::Registry().get<Sprite>(com.currentStaminaBar);
				com.staminaBarOffset -= (sprite.size.x - (BAR_START_SIZE * (com.currentStamina / com.maxStamina))) / 2;
				sprite.size.x = BAR_START_SIZE * (com.currentStamina / com.maxStamina);
				if (com.currentStamina < 0.f) {
					com.currentStamina = 0.f;
				}
				if (weapon.currentTimer >= weapon.animTimer || com.currentStamina == 0.f) {
					weapon.currentTimer = 0.f;
					weapon.animPlaying = false;
					weapon.attacking = false;
					AssignSprite(weapon_sprite, chr.meleeWeaponSpriteName);
				}
				else {		
					weapon.currentTimer += Engine::DeltaTime();
					continue;
				}
			}
			
			Float32 attack = input.Get("attack");
			if (EPSILON_NOT_ZERO(attack) ) {
				if (chr.attacking) {
					if (!weapon.attacking) {
						auto& anim = Engine::Registry().get_or_emplace<Animator>(chr.weaponSprite);
						AnimatorPlay(anim, chr.weaponAnimationName);
						weapon.animPlaying = true;
						weapon.attacking = true;
					}
				}
				else {
					weapon.attacking = true;
					chr.attacking = true;
					auto& weapon_sprt = Engine::Registry().get<Sprite>(chr.weaponSprite);
					AssignSprite(weapon_sprt, chr.meleeWeaponSpriteName);
					auto& anim = Engine::Registry().get_or_emplace<Animator>(chr.weaponSprite);
					AnimatorPlay(anim, chr.weaponAnimationName);
					weapon.animPlaying = true;
				}

			}
		}
	}
}

void MeleeSystem::SpinUp()
{
	Engine::Dispatcher().sink<NextFrame>().connect<&MeleeSystem::OnEndOfFrame>(this);

}

void MeleeSystem::OnEndOfFrame() {
	auto& view = Engine::Registry().view<Weapon>();
	for (auto entity : view) {
		auto& weapon = view.get<Weapon>(entity);
		auto entity_chr = weapon.holder;
		auto& sprite = Engine::Registry().get<Sprite>(entity);
		auto& chr = Engine::Registry().get<PlightCharacterController>(entity_chr);
		if (!weapon.attacking) {
			if (!chr.attacking) {
				AssignSprite(sprite, chr.weaponSpriteName);
			}
			else {
				AssignSprite(sprite, chr.meleeWeaponSpriteName);
			}
			Engine::Registry().remove_if_exists<Animator>(entity);
			
		}
	}
}
void MeleeSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&MeleeSystem::OnEndOfFrame>(this);
}