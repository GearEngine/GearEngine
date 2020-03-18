#pragma once
#include "glm/glm.hpp"

namespace InGame {

	namespace EventType
	{
		enum : unsigned int
		{
			Explosion,
		};
	}

	struct ExplosionData
	{
		ExplosionData(const glm::vec2 position, float radius, float power)
			: Position(position), radius(radius), Power(power)
		{
		}
		glm::vec2 Position;
		float radius;
		float Power;
	};

}

