#pragma once

#include "Component.h"
#include "box2d/box2d.h"

namespace Gear {
	class PhysicsWorld
	{
	private:
		PhysicsWorld();
		~PhysicsWorld();

	public:

	private:
		static b2World* s_World;
	};


	class Physics : public Component
	{

		
	public:

	};

}