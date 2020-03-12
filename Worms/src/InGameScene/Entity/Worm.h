#pragma once
#include "Gear.h"

class Worm : public Gear::Entity2D
{
public:
	Worm(const glm::vec3& position, const float rotation, const glm::vec2 scale);
	virtual void Update(Gear::Timestep ts) override;
	virtual void Render() override;
};