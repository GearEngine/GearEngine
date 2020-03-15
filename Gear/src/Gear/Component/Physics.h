#pragma once

#include "Component.h"

namespace Gear {

	class Physics : public Component
	{
	public:
		virtual ~Physics() = default;

	private:
		virtual void Update(Timestep ts) override;

	private:
		friend class EntitySystem;
	};

}