#pragma once
#include "Gear.h"

class Worm : public Gear::Entity
{
public:
	Worm(const glm::vec3& position, const float rotation, const glm::vec2 scale);
	~Worm();

private:
	int m_ID;
};