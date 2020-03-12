#pragma once

#include "Component.h"
#include "Gear/Renderer/Animation.h"

namespace Gear {

	class Animator2D : public Component
	{
	public:
		Animator2D() = default;
		~Animator2D();

		virtual void Update(Timestep ts) override;
		void AddAnimation(std::string name, Ref<Animation2D> animation);
		void SetCurrentAnimation(const std::string name);
		inline Ref<Animation2D> GetCurrentAnimation() { return m_CurrentAnimation; }

		void PlayAnimation(std::string name);
		void PauseAnimation();
		void StopAnimation();
		void ResumeAnimation();
		void SetFrameX(int frameX);
		void SetFrameY(int frameY);

	private:
		std::unordered_map<std::string, Ref<Animation2D>> m_AnimationList;
		Ref<Animation2D> m_CurrentAnimation;
		friend class Drawer;
	};

}