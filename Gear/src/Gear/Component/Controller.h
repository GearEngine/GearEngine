#pragma once

#include "Component.h"
#include "Gear/Manager/NetWorkManager.h"

namespace Gear {

	struct Command : public PacketAble
	{
		Command() = default;
		Command(EnumType keytype, int keycode)
			: KeyType(keytype), Keycode(keycode)
		{}
		EnumType KeyType;
		int Keycode;
		glm::vec2 MouseMove;


		bool operator==(const Command& cmd)
		{
			if (cmd.KeyType == KeyType && cmd.Keycode == Keycode)
			{
				return true;
			}
			return false;
		}

		virtual void Read(InputMemoryStream& stream) override
		{
			stream.Read((unsigned int)KeyType);
			stream.Read(Keycode);
		}
		virtual void Write(OutputMemoryStream& stream) override
		{
			stream.Write((unsigned int)KeyType);
			stream.Write(Keycode);
		}
	};

	#define MOUSEMOVE_COMMAND 0xffffff
	#define MOUSE_CLICKTYPE   0xfffffc
	#define NONE_COMMAND      0xffffffb

	class Controller : public Component
	{
	public:
		Controller(int entityID)
			: Component(entityID)
		{}
		virtual ~Controller();

	private:
		virtual void Update(Timestep ts) override;
		inline void RegisterCommand(const std::initializer_list<Command>& commands) { m_Commands = commands; }
		inline void ActivateMouse(bool activate) { m_ActivatedMouse = activate; }
		inline const Command& GetCommand() const { return m_Command; }
		inline void ResetCommand() { m_Command = s_None; }

	private:
		bool m_ActivatedMouse = false;

		std::vector<Command> m_Commands;
		static Command s_None;
		Command m_Command;

		std::pair<float, float> m_PrevMousePos = {0.0f, 0.0f};

		friend class EntitySystem;
		friend class NetController;
	};

	class NetController : public Component
	{
	public:
		NetController(int entityID)
			: Component(entityID)
		{}
		virtual ~NetController();
	public:
		virtual void Update(Timestep ts) override;

		void SendInput();
		void ReceiveInput();
		inline void RegisterCommand(const std::initializer_list<Command>& commands) { m_Commands = commands; }
		inline void ActivateMouse(bool activate) { m_ActivatedMouse = activate; }
		inline const Command& GetCommand() const { return m_Command; }
		inline void ResetCommand() { m_Command = Controller::s_None; temp = Controller::s_None; }

	private:
		std::vector<Command> m_Commands;
		Command temp;
		Command m_Command;
		bool m_ActivatedMouse = false;

		friend class EntitySystem;
	};

}
