#pragma once

#include "Component.h"

namespace Gear {

	struct Command;

	class FSM : public Component
	{
	public:
		class InputHandler
		{
		public:
			virtual EnumType Handle(int entityID, const Command& cmd) = 0;
		};

	public:
		virtual ~FSM();

	private:
		FSM() = default;
		virtual void Update(Timestep ts) override;

		void RegisterFSM(const std::initializer_list<std::pair<const EnumType, InputHandler*>>& handlers);
		void Handle(int entityID, const Command& cmd);
		inline EnumType GetCurrentState() const { return m_CurrentState; }

	private:
		std::vector<EnumType> m_States;
		std::unordered_map<EnumType, InputHandler*> m_Handlers;
		EnumType m_CurrentState = 0;

		friend class EntitySystem;
	};
	
}