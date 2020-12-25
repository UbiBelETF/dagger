#define _USE_MATH_DEFINES


#include "plight_fields.h"


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

void FieldsSystem::Run()
{
	auto viewLS1 = Engine::Registry().view<PlightIntro>();
	auto it = viewLS1.begin();
	auto& pin = viewLS1.get<PlightIntro>(*it);

	if (pin.IsFinished())
	{
		//Checking input for fields for all the players
		auto view = Engine::Registry().view<PlightCharacterController, CombatStats,InputReceiver,Transform,DefenseField>();
		for (auto entity : view) {
			auto& character = view.get<PlightCharacterController>(entity);
			auto& t = view.get<Transform>(entity);
			auto& cstats = view.get<CombatStats>(entity);
			auto& input = view.get<InputReceiver>(entity);
			auto& defenseField = view.get<DefenseField>(entity);

			if(defenseField.fieldsLeft > 0){
			if (defenseField.fieldActive) {
				defenseField.currendDurationTime += Engine::DeltaTime();
				if (defenseField.currendDurationTime > defenseField.fieldDurationTime / 15 && defenseField.currendDurationTime < (defenseField.fieldDurationTime/15)*14) {
					auto& anim = Engine::Registry().get<Animator>(defenseField.defenseFieldE);
					AnimatorPlay(anim, "Plight:fields:DEFENSE_FIELD");
				}
				else if (defenseField.currendDurationTime >= (defenseField.fieldDurationTime / 15) * 14 && defenseField.currendDurationTime < defenseField.fieldDurationTime) {
						auto& anim = Engine::Registry().get<Animator>(defenseField.defenseFieldE);
						AnimatorPlay(anim, "Plight:fields:DEFENSE_FIELD_DESPAWN");
				}
				else if(defenseField.currendDurationTime > defenseField.fieldDurationTime){
						defenseField.destroy = true;
						defenseField.fieldsLeft--;
						cstats.healing = false;
						defenseField.currendDurationTime = 0.f;
				}
				continue;
			}

			Float32 defend = input.Get("defense_field");
			if (EPSILON_NOT_ZERO(defend)) {
				if (cstats.currentStamina > defenseField.initialStaminaCost) {
					defenseField.fieldActive = true;
					auto& defSprite = Engine::Registry().get_or_emplace<Sprite>(defenseField.defenseFieldE);
					AssignSprite(defSprite, "Plight:fields:SPAWN:defensefield_spawn_1");
					defSprite.scale = { 0.4f,0.4f };
					auto& defTransform = Engine::Registry().get_or_emplace<Transform>(defenseField.defenseFieldE);
					auto& defPhysics = Engine::Registry().emplace<PhysicsObject>(defenseField.defenseFieldE);
					auto& defCollision = Engine::Registry().emplace<PlightCollision>(defenseField.defenseFieldE);
					auto& anim = Engine::Registry().emplace<Animator>(defenseField.defenseFieldE);

					
					defSprite.position = t.position;
					defTransform.position = t.position;
					defSprite.position.z = 70.f;
					defTransform.position.z = 70.f;
					defPhysics.my_groups.push_back(1);
					defPhysics.collision_groups.push_back(1);
					defCollision.size = { defenseField.fieldSize,defenseField.fieldSize };
					AnimatorPlay(anim, "Plight:fields:DEFENSE_FIELD_SPAWN");
					cstats.healing = true;
				}
			}
			}
		}
	}
}

void FieldsSystem::SpinUp()
{
	Engine::Dispatcher().sink<NextFrame>().connect<&FieldsSystem::OnEndOfFrame>(this);

}

void FieldsSystem::OnEndOfFrame() {
	auto view = Engine::Registry().view<DefenseField>();
	for (auto entity : view) {
		auto& defenseField = view.get<DefenseField>(entity);
		if (defenseField.destroy) {
			defenseField.fieldActive = false;
			defenseField.destroy = false;
			auto& anim = Engine::Registry().get<Animator>(defenseField.defenseFieldE);
			AnimatorStop(anim);
			Engine::Registry().remove_all(defenseField.defenseFieldE);

		}
	}
}
void FieldsSystem::WindDown()
{
	Engine::Dispatcher().sink<NextFrame>().disconnect<&FieldsSystem::OnEndOfFrame>(this);
}