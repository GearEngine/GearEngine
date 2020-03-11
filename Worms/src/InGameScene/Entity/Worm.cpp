#include "Worm.h"

enum WormState
{
	Walk,
	Attaked,
	Dye
};

Worm::Worm(const glm::vec3 & position)
{
	AttatchAnimator();
	AttatchPhysics();
	AttatchSoundPlayer();
	AttatchState();
	AttatchTransform();
	AttatchController();

	m_Transform->SetPosition(position);
	m_State->RegisterStates({
		{ Walk, "Walk" },
		{ Attaked, "Attaked" },
		{ Dye, "Dye" }
	});
}

void Worm::update()
{
}

void Worm::Render()
{
}
