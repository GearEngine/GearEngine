#pragma once
#include "Gear.h"

class Worm : public Gear::Entity2D
{
public:
	Worm(const glm::vec3& position, const float rotation, const glm::vec2 scale);
	void update(Gear::Timestep ts);
	void Render();
};