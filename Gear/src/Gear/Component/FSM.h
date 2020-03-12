#pragma once

#include "Component.h"

namespace Gear {

	class Command;

	class FSM : public Component
	{
		using StateEnumType = int;
	public:
		class InputHandler
		{
		public:
			virtual int Handle(Command cmd) = 0;
		};

		class State
		{
		public:
			void RegisterStates(const std::initializer_list<std::pair<StateEnumType, std::string>>& states);
			inline std::string GetCurrentState() { return m_States[m_CurrentState]; }
			inline void SetCurrentState(StateEnumType state) { m_CurrentState = state; }

		private:
			std::unordered_map<StateEnumType, std::string> m_States;

			int m_CurrentState = 0;
		};

	public:
		virtual ~FSM();
		virtual void Update(Timestep ts) override;

		void Handle(Command cmd);
		void RegisterHandler(const std::initializer_list < std::pair<StateEnumType, InputHandler*>>& handlers);
		State& GetState(){ return m_State; }

	private:
		State m_State;
		std::unordered_map<StateEnumType, InputHandler*> m_Handlers;
		int m_CurrentState;
	};
	
}