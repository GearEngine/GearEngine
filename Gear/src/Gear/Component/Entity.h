#pragma once

namespace Gear {

	class Entity
	{
	public:
		Entity() {}
	private:
		Entity(int entityID)
			: m_EntityID(entityID)
		{}

	protected:
		int m_EntityID;
		bool m_OnActivate;

		std::queue<Ref<struct EntityEvent>> m_EventQueue;
		std::unordered_map<EnumType, Ref<class EventHandler>> m_EventHandler;

		friend class EntitySystem;
		friend class EventSystem;
		friend class EventChannel;
	};

}