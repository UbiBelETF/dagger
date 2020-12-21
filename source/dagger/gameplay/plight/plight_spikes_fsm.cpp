#define _USE_MATH_DEFINES

#include "plight_spikes_fsm.h"
#include "core/core.h"
#include "core/engine.h"
#include "core/input/inputs.h"
#include "core/graphics/sprite.h"
#include "core/game/transforms.h"
#include "core/graphics/animation.h"

#include "gameplay/plight/plight_spikes.h"
#include "gameplay/plight/plight_collisions.h"
#include "gameplay/plight/plight_controller.h"
#include "gameplay/plight/plight_combat.h"

using namespace dagger;
using namespace plight;


// Inactive

void PlightSpikesFSM::Inactive::Enter(PlightSpikesFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "Plight:environment:SPIKES_INACTIVE");
	auto& spikes = Engine::Registry().get<PlightSpikes>(state_.entity);
}

DEFAULT_EXIT(PlightSpikesFSM, Inactive);

void PlightSpikesFSM::Inactive::Run(PlightSpikesFSM::StateComponent& state_)
{
	
		auto& col = Engine::Registry().get<PlightCollision>(state_.entity);
		auto& spikes = Engine::Registry().get<PlightSpikes>(state_.entity);
		if (spikes.activated) {
			spikes.currentResetTimer += Engine::DeltaTime();
			if (spikes.currentResetTimer >= spikes.spikesResetTimer) {
				spikes.activated = false;
				spikes.currentResetTimer = 0.f;
			}
		}
		else if (col.colided) {
			auto it = col.colidedWith.begin();
			while (it != col.colidedWith.end()) {
				if (Engine::Registry().valid(*it)) {
					if (Engine::Registry().has<PlightCharacterController>(*it)) {
						auto& ch = Engine::Registry().get<CombatStats>(*it);
						auto& pchar = Engine::Registry().get<PlightCharacterController>(*it);

						ch.currentHealth -= 10.f;

						if (ch.currentHealth <= 0.f) {
							ch.currentHealth = 0.f;
						}
						auto& sprite = Engine::Registry().get<Sprite>(ch.currentHealthBar);
						ch.healthBarOffset += (sprite.size.x - (BAR_START_SIZE * (ch.currentHealth / ch.maxHealth))) / 2;
						sprite.size.x = BAR_START_SIZE * (ch.currentHealth / ch.maxHealth);

						pchar.hit = true;

						spikes.activated = true;
						GoTo(PlightSpikesStates::Active, state_);
						break;
					}

				}
				it++;
			}
		}
	

}


// Active


void PlightSpikesFSM::Active::Enter(PlightSpikesFSM::StateComponent& state_)
{
	auto& animator = Engine::Registry().get<Animator>(state_.entity);
	AnimatorPlay(animator, "Plight:environment:SPIKES_ACTIVE");
}

DEFAULT_EXIT(PlightSpikesFSM, Active);

void PlightSpikesFSM::Active::Run(PlightSpikesFSM::StateComponent& state_)
{
	auto& spikes = Engine::Registry().get<PlightSpikes>(state_.entity);
	spikes.currentAnimationTimer += Engine::DeltaTime();
	if (spikes.currentAnimationTimer >= spikes.animationTimer) {
		spikes.currentAnimationTimer = 0.f;
		GoTo(PlightSpikesStates::Inactive, state_);
	}
}