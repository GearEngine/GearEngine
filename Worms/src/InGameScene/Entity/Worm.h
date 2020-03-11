#pragma once
#include "Gear.h"

class Worm : public Gear::Entity2D
{
public:
	Worm(const glm::vec3& position);
	void update();
	void Render();
};