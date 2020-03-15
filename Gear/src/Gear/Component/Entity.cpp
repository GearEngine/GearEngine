#include "grpch.h"
#include "Entity.h"
#include "EntitySystem.h"

namespace Gear {

	void Entity::SendMessage_(const std::string message)
	{
		Gear::EntitySystem::DispatchMessage_(m_EntityID, message);
	}

	void Entity::ReceiveMessage_(int from, const std::string message)
	{
		std::cout << "On " << m_EntityID;
		std::cout << "from Entity" << from << std::endl;
		std::cout << message << std::endl;
	}
}