#pragma once

#include "core/core.h"
#include "core/system.h"

#include <limits>

template<typename States, ENUM_ONLY(States)>
struct FSM
{
	struct StateComponent
	{
		Entity entity;
		States currentState;
	};

	struct State
	{
		inline virtual void Enter(StateComponent&) {}
		inline virtual void Run(StateComponent&) {}
		inline virtual void Exit(StateComponent&) {}

		State(FSM<States>* parent_)
			: m_Parent{ parent_ }
		{}

		inline void GoTo(States nextState_, StateComponent& component_)
		{
			assert(m_Parent != nullptr);
			m_Parent->GoTo(nextState_, component_);
		}

	private:
		ViewPtr<FSM<States>> m_Parent;
	};

	inline void GoTo(States nextState_, StateComponent& component_)
	{
		auto currentState = component_.currentState;
		if (currentState != nextState_)
		{
			m_StatePointers[currentState]->Exit(component_);
			currentState = component_.currentState = nextState_;
			m_StatePointers[currentState]->Enter(component_);
		}
	}

	inline void Run(StateComponent& component_)
	{
		assert(m_StatePointers.contains(component_.currentState));
		m_StatePointers[component_.currentState]->Run(component_);
	}

protected:
	const UInt32 stateSize = (UInt32) std::numeric_limits<enum States>::max();

	Map<States, OwningPtr<State>> m_StatePointers;
};

#define MAP_STATE_TO_CLASS(State_, Class_) m_StatePointers[State_] = std::move(OwningPtr<Class_>(new Class_(this)))

#define DEFINE_STATE(Machine_, States_, State_) struct State_ : public State \
{ \
	virtual void Enter(StateComponent& component_) override; \
	virtual void Run(StateComponent& component_) override; \
	virtual void Exit(StateComponent& component_) override; \
	State_(FSM<States_>* parent_) : State(parent_) {} \
};

#define ATTACH_TO_FSM(Machine_, Entity_) Engine::Registry().get_or_emplace<Machine_::StateComponent>(Entity_, Entity_);

#define DEFAULT_ENTER(Machine_, State_) void Machine_::State_::Enter(Machine_::StateComponent&) {}
#define DEFAULT_EXIT(Machine_, State_) void Machine_::State_::Exit(Machine_::StateComponent&) {}
#define DEFAULT_RUN(Machine_, State_) void Machine_::State_::Run(Machine_::StateComponent&) {}
