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
		const States m_State;

		State(FSM<States>* parent_, States state_)
			: m_Parent{ parent_ }
			, m_State{ state_ }
		{}

		inline void GoTo(States nextState_, StateComponent& component_)
		{
			assert(m_Parent != nullptr);
			m_Parent->GoTo(nextState_, component_);
		}

		FSM<States>* GetFSM()
		{
			return m_Parent.get();
		}

		State* GetState() const
		{
			return m_Parent->GetStates()[m_State];
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

	inline virtual void Run(StateComponent& component_)
	{
		assert(m_StatePointers.contains(component_.currentState));
		m_StatePointers[component_.currentState]->Run(component_);
	}

	inline Map<States, OwningPtr<State>> GetStates() const
	{
		return m_StatePointers;
	}

protected:
	const UInt32 stateSize = (UInt32)std::numeric_limits<States>::max();

	Map<States, OwningPtr<State>> m_StatePointers;
};

#define CONNECT_STATE(States_, Class_) m_StatePointers[States_::Class_] = std::move(OwningPtr<Class_>(new Class_(this)))

#define DEFINE_STATE(Machine_, States_, State_) struct State_ : public State \
{ \
	virtual void Enter(StateComponent& component_) override; \
	virtual void Run(StateComponent& component_) override; \
	virtual void Exit(StateComponent& component_) override; \
	State_(FSM<States_>* parent_) : State(parent_, States_::State_) {} \
};

#define ATTACH_TO_FSM(Machine_, Entity_) Engine::Registry().get_or_emplace<Machine_::StateComponent>(Entity_, Entity_);

#define DEFAULT_ENTER(Machine_, State_) void Machine_::State_::Enter(Machine_::StateComponent&) {}
#define DEFAULT_EXIT(Machine_, State_) void Machine_::State_::Exit(Machine_::StateComponent&) {}
#define DEFAULT_RUN(Machine_, State_) void Machine_::State_::Run(Machine_::StateComponent&) {}