#include "pingpong_playerinput.h"

#include "core/engine.h"
#include "core/game/transforms.h"

using namespace dagger;
using namespace ping_pong;

Float32 PingPongPlayerInputSystem::s_BoarderDown = -20;
Float32 PingPongPlayerInputSystem::s_BoarderUp = 20;

Float32 PingPongPlayerInputSystem::s_PlayerSpeed = 1.f;

void PingPongPlayerInputSystem::SpinUp()
{
    Engine::Dispatcher().sink<KeyboardEvent>().connect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
}

void PingPongPlayerInputSystem::WindDown()
{
    Engine::Dispatcher().sink<KeyboardEvent>().disconnect<&PingPongPlayerInputSystem::OnKeyboardEvent>(this);
}

void PingPongPlayerInputSystem::OnKeyboardEvent(KeyboardEvent kEvent_)
{
	Engine::Registry().view<ControllerMapping>().each([&](ControllerMapping& ctrl_)
	{
		if (ctrl_.frozen) {
			return;
		} // If a player is already frozen stop any movement actions
		
		if (kEvent_.key == ctrl_.up_key && (kEvent_.action == EDaggerInputState::Pressed || kEvent_.action == EDaggerInputState::Held))
		{
			ctrl_.input.y = 1;
		}
		else if (kEvent_.key == ctrl_.up_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y > 0)
		{
			ctrl_.input.y = 0;
		}
		else if (kEvent_.key == ctrl_.down_key && (kEvent_.action == EDaggerInputState::Held || kEvent_.action == EDaggerInputState::Pressed))
		{
			ctrl_.input.y = -1;
		}
		else if (kEvent_.key == ctrl_.down_key && kEvent_.action == EDaggerInputState::Released && ctrl_.input.y < 0)
		{
			ctrl_.input.y = 0;
		}

		if (kEvent_.key == ctrl_.boost_key && kEvent_.action == EDaggerInputState::Pressed) {
			ctrl_.boosting = !ctrl_.boosting;	
		}
		
    });
}

void PingPongPlayerInputSystem::Run()
{
    auto view = Engine::Registry().view<Transform, ControllerMapping>();
	for (auto entity : view)
	{
		auto &t = view.get<Transform>(entity);
		auto &ctrl = view.get<ControllerMapping>(entity);

		if (ctrl.frozen) {// If frozen player is unable to move
			if ((ctrl.frozenFor -= Engine::DeltaTime()) <= 0) {
				ctrl.frozen = false;  // Allowed to move again
				ctrl.boosting = false;// Stopped the boost, in case player hit the ball while boosting
			}
			continue;
		}

		auto move = ctrl.input.y * s_PlayerSpeed * Engine::DeltaTime();
		if (ctrl.boosting) move *= ControllerMapping::s_boostSpeedIncrease;

		t.position.y += move;

		if (t.position.y > s_BoarderUp)
		{
			t.position.y = s_BoarderUp;
		}

		if (t.position.y < s_BoarderDown)
		{
				t.position.y = s_BoarderDown;
		}

		if (ctrl.boosting && (ctrl.boostLeft -= Engine::DeltaTime()) <= 0) { // If player is not boosting AND fails on first condititon and wont 
			ctrl.frozen = true;												// check second condition and decrese boostLeft time
			ctrl.frozenFor = ControllerMapping::s_boostMaxTime;
			ctrl.input = { 0,0 };
			// Freeze as a penalty and reset boost
			ctrl.boostLeft = ControllerMapping::s_boostMaxTime;
			ctrl.boosting = false;
		}		
    }
}
