#pragma once

#include "core/core.h"
#include "core/system.h"

#include <limits>

template<typename States, ENUM_ONLY(States)>
struct FiniteStateComponent
{
	States currentState;
};

template<typename States, typename DataComponent, ENUM_ONLY(States), IS_A(DataComponent, FiniteStateComponent<States>)>
struct FiniteStateMachine
{
	using FSM = FiniteStateMachine<States, DataComponent>;

	struct State
	{
		inline virtual void Enter(DataComponent& component_) {}
		inline virtual void Run(DataComponent& component_) {}
		inline virtual void Exit(DataComponent& component_) {}

		State(FSM* parent_)
			: m_Parent{ parent_ }
		{}

		inline void GoTo(States nextState_, DataComponent& component_)
		{
			assert(m_Parent != nullptr);
			m_Parent->GoTo(nextState_, component_);
		}

	private:
		ViewPtr<FSM> m_Parent;
	};

	inline void GoTo(States nextState_, DataComponent& component_)
	{
		auto currentState = component_.currentState;
		if (currentState != nextState_)
		{
			m_StatePointers[currentState]->Exit(component_);
			currentState = component_.currentState = nextState_;
			m_StatePointers[currentState]->Enter(component_);
		}
	}

	inline void Run(DataComponent& component_)
	{
		m_StatePointers[component_.currentState]->Run(component_);
	}

	template<typename... T>
	inline void Run(Registry& registry_)
	{
		registry_.view<DataComponent, T...>().each([&](DataComponent& component_, T...&)
			{
				Run(component_);
			});
	}

protected:
	const UInt32 stateSize = (UInt32) std::numeric_limits<enum States>::max();

	Map<States, OwningPtr<State>> m_StatePointers;
};

#define MAP_STATE_TO_CLASS(State_, Class_) m_StatePointers[State_] = std::move(OwningPtr<Class_>(new Class_(this)))
