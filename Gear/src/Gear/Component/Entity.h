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

	public:
		void SendMessage_(const std::string message);
		void ReceiveMessage_(int from, const std::string message);

	protected:
		int m_EntityID;
		bool m_OnActivate;

		friend class EntitySystem;
	};

}