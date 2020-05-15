#pragma once

namespace Gear {

	using EntityEventType = unsigned int;
	struct EntityEvent
	{
		EntityEvent(EntityEventType type, const std::any& data)
			: Type(type), Data(data)
		{}
		EntityEventType Type;
		std::any Data;
		bool handled = false;
	};

	class Entity
	{
	public:
		Entity() 
		{
			for (auto handler = m_EventHandler.begin(); handler != m_EventHandler.end(); ++handler)
			{
				handler->second.reset();
			}
		}
	private:
		Entity(int entityID, const std::string& name = "")
			: m_EntityID(entityID), m_Name(name)
		{}

	public:
		const std::string& GetName() const { return m_Name; }

	protected:
		int m_EntityID;
		bool m_OnActivate = false;
		std::string m_Name;

		std::vector<struct EntityEvent> m_EventQueue;
		std::unordered_map<EnumType, Ref<class EventHandler>> m_EventHandler;

		friend class EntitySystem;
		friend class EventSystem;
		friend class EventChannel;
	};

}