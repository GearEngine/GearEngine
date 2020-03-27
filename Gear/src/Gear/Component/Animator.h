#pragma once

#include "Component.h"
#include "Gear/Renderer/Animation.h"

namespace Gear {

	class Animator : public Component
	{
	protected:
		Animator(int id)
			: Component(id)
		{}
	private:
		virtual void PlayAnimation(EnumType name) = 0;
		virtual void PauseAnimation() = 0;
		virtual void StopAnimation() = 0 ;
		virtual void ResumeAnimation() = 0;
	};

	class Animator2D : public Animator
	{
	public:
		Animator2D(int id)
			: Animator(id)
		{}
		virtual ~Animator2D();

	private:
		virtual void Update(Timestep ts) override;
		void RegisterAnimation(const std::initializer_list < std::pair<const EnumType, Ref<Animation2D>>>& animationList);

	public:
		void SetCurrentAnimation(EnumType name);
		inline Ref<Animation2D> GetCurrentAnimation() { return m_CurrentAnimation; }

		virtual void PlayAnimation(EnumType name) override;
		virtual void PauseAnimation() override;
		virtual void StopAnimation() override;
		virtual void ResumeAnimation() override;

		void SetFrameX(int frameX);
		void SetFrameY(int frameY);

	private:
		std::unordered_map<EnumType, Ref<Animation2D>> m_AnimationList;
		Ref<Animation2D> m_CurrentAnimation;

		friend class EntitySystem;
	};

}