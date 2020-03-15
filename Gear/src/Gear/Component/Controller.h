#pragma once

#include "Component.h"

namespace Gear {

	struct Command
	{
		Command() = default;
		Command(EnumType keytype, int keycode)
			: KeyType(keytype), Keycode(keycode)
		{}
		EnumType KeyType;
		int Keycode;
	};

	class Controller : public Component
	{
	public:
		virtual ~Controller();

	private:
		virtual void Update(Timestep ts) override;
		inline void RegisterCommand(const std::initializer_list<Command>& commands) { m_Commands = commands; }
		inline const Command& GetCommand() const { return m_Command; }

	private:
		std::vector<Command> m_Commands;
		Command m_None = { -1, -1 };
		Command m_Command;

		friend class EntitySystem;
	};

}
