#pragma once

#include "Animation.h"

namespace Gear {

	class Animator2D
	{
	public:
		Animator2D() = default;
		~Animator2D();

		void AddAnimation(std::string name, Ref<Animation2D> animation);
		void Update(Timestep ts);

		void PlayAnimation(std::string name);
		void PauseAnimation();
		void StopAnimation();
		void ResumeAnimation();

		void Render();
		void Bind(uint32_t slot);

	private:
		std::unordered_map<std::string, Ref<Animation2D>> m_AnimationList;
		Ref<Animation2D> m_CurrentAnimation;
		
	};

}