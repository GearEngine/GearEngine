#pragma once

#include "Component.h"
#include "Gear/Manager/NetWorkManager.h"

namespace Gear {

	struct Command : public PacketAble
	{
		Command()
		{
			m_Type = 0;
		};
		Command(EnumType keytype, int keycode)
			: KeyType(keytype), Keycode(keycode)
		{
			m_Type = 0;
		}
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
			stream.Read(m_Type);
			stream.Read((unsigned int)KeyType);
			stream.Read(Keycode);
		}
		virtual void Write(OutputMemoryStream& stream) override
		{
			stream.Write(m_Type);
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


	struct TypeChecker : public PacketAble
	{
		virtual void Read(InputMemoryStream& stream) override
		{
			stream.Read(m_Type);
		}
		virtual void Write(OutputMemoryStream& stream) override
		{
		}
	};

	class NetController : public Component
	{
	public:
		class EventController
		{
		public:
			virtual void Handle(int entityID, InputMemoryStream& data) = 0;
		};

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
		inline void ResetCommand() { m_Command = Controller::s_None; }
		inline void RegisterEventCotroller(Ref<EventController> eventControllers)
		{
			m_EventCotroller = eventControllers;
		}
		inline void pushAccepterbleEntity(int entityID)
		{
			accepterbleEntity.push_back(entityID);
		}

	private:
		std::vector<Command> m_Commands;
		Ref<EventController> m_EventCotroller;
		std::vector<int> accepterbleEntity;
		Command m_Command;
		TypeChecker m_TypeChecker;
		bool m_ActivatedMouse = false;

		friend class EntitySystem;
	};

}
