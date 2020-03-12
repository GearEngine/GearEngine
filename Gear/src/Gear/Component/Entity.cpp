#include "grpch.h"
#include "Entity.h"

namespace Gear {
	Entity2D::~Entity2D()
	{
		DetatchSoundPlayer();
		DetatchFSM();
		DetatchTransform();
		DetatchAnimator();
		DetatchPhysics();
		DetatchController();
	}
	void Entity2D::AttatchSoundPlayer()
	{
		if (!m_SoundPlayer)
			m_SoundPlayer = new SoundPlayer;
	}
	void Entity2D::AttatchFSM()
	{
		if (!m_FSM)
			m_FSM = new FSM;
	}
	void Entity2D::AttatchTransform()
	{
		if (!m_Transform)
			m_Transform = new Transform2D;
	}
	void Entity2D::AttatchAnimator()
	{
		if (!m_Animator)
			m_Animator = new Animator2D;
	}
	void Entity2D::AttatchPhysics()
	{
		if (!m_Physics)
			m_Physics = new Physics;
	}
	void Entity2D::AttatchController()
	{
		if (!m_Controller)
			m_Controller = new Controller;
	}

	void Entity2D::DetatchSoundPlayer()
	{
		if (m_SoundPlayer)
			delete m_SoundPlayer;
	}
	void Entity2D::DetatchFSM()
	{
		if (m_FSM)
			delete m_FSM;
	}
	void Entity2D::DetatchTransform()
	{
		if (m_Transform)
			delete m_Transform;
	}
	void Entity2D::DetatchAnimator()
	{
		if (m_Animator)
			delete m_Animator;
	}
	void Entity2D::DetatchController()
	{
		if (m_Controller)
			delete m_Controller;
	}
	void Entity2D::DetatchPhysics()
	{
		if (m_Physics)
			delete m_Physics;
	}

	void Entity2D::ActivateSoundPlayer()
	{
		if (m_SoundPlayer)
			m_SoundPlayer->Activate();
	}
	void Entity2D::ActivateFSM()
	{
		if (m_FSM)
			m_FSM->Activate();
	}
	void Entity2D::ActivateTransform()
	{
		if (m_Transform)
			m_Transform->Activate();
	}
	void Entity2D::ActivateAnimator()
	{
		if (m_Animator)
			m_Animator->Activate();
	}
	void Entity2D::ActivatePhysics()
	{
		if (m_Physics)
			m_Physics->Activate();
	}
	void Entity2D::ActivateController()
	{
		if (m_Controller)
			m_Controller->Activate();
	}

	void Entity2D::InActivateSoundPlayer()
	{
		if (m_SoundPlayer)
			m_SoundPlayer->InActivate();
	}
	void Entity2D::InActivateFSM()
	{
		if (m_FSM)
			m_FSM->InActivate();
	}
	void Entity2D::InActivateTransform()
	{
		if (m_Transform)
			m_Transform->InActivate();
	}
	void Entity2D::InActivatePhysics()
	{
		if (m_Physics)
			m_Physics->InActivate();
	}
	void Entity2D::InActivateAnimator()
	{
		if (m_Animator)
			m_Animator->InActivate();
	}
	void Entity2D::InActivateController()
	{
		if (m_Controller)
			m_Controller->InActivate();
	}
}