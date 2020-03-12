#pragma once

#include "SoundPlayer.h"
#include "Animator.h"
#include "Physics.h"
#include "FSM.h"
#include "Transform.h"
#include "Controller.h"
#include "Drawer.h"

namespace Gear {

	class Entity 
	{
	public:
		virtual void AttatchSoundPlayer() = 0;
		virtual void AttatchFSM() = 0;
		virtual void AttatchTransform() = 0;
		virtual void AttatchAnimator() = 0;
		virtual void AttatchPhysics() = 0;
		virtual void AttatchController() = 0;
		virtual void AttatchDrawer() = 0;

		virtual void DetatchSoundPlayer() = 0;
		virtual void DetatchFSM() = 0;
		virtual void DetatchTransform() = 0;
		virtual void DetatchAnimator() = 0;
		virtual void DetatchPhysics() = 0;
		virtual void DetatchController() = 0;
		virtual void DetatchDrawer() = 0;

		virtual void ActivateSoundPlayer() = 0;
		virtual void ActivateFSM() = 0;
		virtual void ActivateTransform() = 0;
		virtual void ActivatePhysics() = 0;
		virtual void ActivateAnimator() = 0;
		virtual void ActivateController() = 0;
		virtual void ActivateDrawer() = 0;

		virtual void InActivateSoundPlayer() = 0;
		virtual void InActivateFSM() = 0;
		virtual void InActivateTransform() = 0;
		virtual void InActivatePhysics() = 0;
		virtual void InActivateAnimator() = 0;
		virtual void InActivateController() = 0;
		virtual void InActivateDrawer() = 0;
	};

	class Entity2D : public Entity
	{
	public:
		virtual ~Entity2D();

		virtual void AttatchSoundPlayer() override;
		virtual void AttatchFSM() override;
		virtual void AttatchTransform() override;
		virtual void AttatchAnimator() override;
		virtual void AttatchPhysics() override;
		virtual void AttatchController() override;
		virtual void AttatchDrawer() override;

		virtual void DetatchSoundPlayer() override;
		virtual void DetatchFSM() override;
		virtual void DetatchTransform() override;
		virtual void DetatchAnimator() override;
		virtual void DetatchPhysics() override;
		virtual void DetatchController() override;
		virtual void DetatchDrawer() override;

		virtual void ActivateSoundPlayer() override;
		virtual void ActivateFSM() override;
		virtual void ActivateTransform() override;
		virtual void ActivateAnimator() override;
		virtual void ActivatePhysics() override;
		virtual void ActivateController() override;
		virtual void ActivateDrawer() override;

		virtual void InActivateSoundPlayer() override;
		virtual void InActivateFSM() override;
		virtual void InActivateTransform() override;
		virtual void InActivatePhysics() override;
		virtual void InActivateAnimator() override;
		virtual void InActivateController() override;
		virtual void InActivateDrawer() override;

	protected:
		Animator2D* m_Animator = nullptr;
		Transform2D* m_Transform = nullptr;
		Physics* m_Physics = nullptr;
		SoundPlayer* m_SoundPlayer = nullptr;
		FSM* m_FSM = nullptr;
		Controller* m_Controller = nullptr;
		Drawer2D* m_Drawer = nullptr;
	};

}