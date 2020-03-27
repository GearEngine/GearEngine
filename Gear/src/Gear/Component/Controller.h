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
		glm::vec2 MouseMove;
	};

	#define MOUSEMOVE_COMMAND 0xffffff
	#define MOUSE_CLICKTYPE   0xfffffc
	class Controller : public Component
	{
	public:
		Controller(int id)
			: Component(id)
		{}
		virtual ~Controller();

	private:
		virtual void Update(Timestep ts) override;
		inline void RegisterCommand(const std::initializer_list<Command>& commands) { m_Commands = commands; }
		inline void ActivateMouseMove() { m_ActivatedMouse = true; }
		inline const Command& GetCommand() const { return m_Command; }
		inline void ResetCommand() { m_Command = s_None; }

	private:
		bool m_ActivatedMouse = false;

		std::vector<Command> m_Commands;
		static Command s_None;
		Command m_Command;

		std::pair<float, float> m_PrevMousePos = {0.0f, 0.0f};

		friend class EntitySystem;
	};

}
