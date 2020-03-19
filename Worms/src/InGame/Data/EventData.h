#pragma once

namespace InGame {

	namespace EventType
	{
		enum : unsigned int
		{
			World,
			Explosion,
			WormMove,
			MouseMove,
			MouseClick
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

	struct MouseMoveData
	{
		MouseMoveData(float _dx, float _dy)
			: dx(_dx), dy(_dy)
		{}
		float dx, dy;
	};

}

