#pragma once

#include "Component.h"

namespace Gear {

	struct Command
	{
		using KeyEnumType = int;
		Command() = default;
		Command(int keytype, int keycode)
			: KeyType(keytype), Keycode(keycode)
		{}
		KeyEnumType KeyType;
		int Keycode;
	};

	class Controller : public Component
	{
	public:
		virtual void Update(Timestep ts) override;
		inline void RegisterCommand(const std::initializer_list<Command>& commands) { m_Commands = commands; }
		inline const Command GetCommand() const { return m_Command; }

	private:
		std::vector<Command> m_Commands;
		Command m_None = { -1, -1 };
		Command m_Command;
	};

}
