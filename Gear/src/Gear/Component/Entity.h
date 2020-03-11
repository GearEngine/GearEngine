#pragma once

#include "SoundPlayer.h"
#include "Animator.h"
#include "Physics.h"
#include "State.h"
#include "Transform.h"
#include "Controller.h"

namespace Gear {

	class Entity 
	{
	public:
		virtual void AttatchSoundPlayer() = 0;
		virtual void AttatchState() = 0;
		virtual void AttatchTransform() = 0;
		virtual void AttatchAnimator() = 0;
		virtual void AttatchPhysics() = 0;
		virtual void AttatchController() = 0;

		virtual void DetatchSoundPlayer() = 0;
		virtual void DetatchState() = 0;
		virtual void DetatchTransform() = 0;
		virtual void DetatchAnimator() = 0;
		virtual void DetatchPhysics() = 0;
		virtual void DetatchController() = 0;

		virtual void ActivateSoundPlayer() = 0;
		virtual void ActivateState() = 0;
		virtual void ActivateTransform() = 0;
		virtual void ActivatePhysics() = 0;
		virtual void ActivateAnimator() = 0;
		virtual void ActivateController() = 0;

		virtual void InActivateSoundPlayer() = 0;
		virtual void InActivateState() = 0;
		virtual void InActivateTransform() = 0;
		virtual void InActivatePhysics() = 0;
		virtual void InActivateAnimator() = 0;
		virtual void InActivateController() = 0;
	};

	class Entity2D : public Entity
	{
	public:
		virtual ~Entity2D();

		virtual void AttatchSoundPlayer() override;
		virtual void AttatchState() override;
		virtual void AttatchTransform() override;
		virtual void AttatchAnimator() override;
		virtual void AttatchPhysics() override;
		virtual void AttatchController() override;

		virtual void DetatchSoundPlayer() override;
		virtual void DetatchState() override;
		virtual void DetatchTransform() override;
		virtual void DetatchAnimator() override;
		virtual void DetatchPhysics() override;
		virtual void DetatchController() override;

		virtual void ActivateSoundPlayer() override;
		virtual void ActivateState() override;
		virtual void ActivateTransform() override;
		virtual void ActivateAnimator() override;
		virtual void ActivatePhysics() override;
		virtual void ActivateController() override;

		virtual void InActivateSoundPlayer() override;
		virtual void InActivateState() override;
		virtual void InActivateTransform() override;
		virtual void InActivatePhysics() override;
		virtual void InActivateAnimator() override;
		virtual void InActivateController() override;

	protected:
		Animator2D* m_Animator = nullptr;
		Transform2D* m_Transform = nullptr;
		Physics* m_Physics = nullptr;
		SoundPlayer* m_SoundPlayer = nullptr;
		State* m_State = nullptr;
		Controller* m_Controller = nullptr;
	};

}