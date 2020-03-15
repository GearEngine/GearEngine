#pragma once
#include "Component.h"

namespace Gear {

	class Drawer : public Component
	{
	private:
		virtual void Render() = 0;
	};

	class Drawer2D : public Drawer
	{
	public:
		virtual ~Drawer2D();

	private:

		virtual void Update(Timestep ts) override;
		virtual void Render() override;

		friend class EntitySystem;
	};


}
