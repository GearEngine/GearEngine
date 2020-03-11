#include "grpch.h"
#include "Animator.h"

namespace Gear {
	Animator2D::~Animator2D()
	{
		m_CurrentAnimation.reset();
		m_AnimationList.clear();
	}

	void Animator2D::AddAnimation(std::string name, Ref<Animation2D> animation)
	{
		m_AnimationList.insert({ name, animation });
	}

	void Animator2D::PlayAnimation(std::string name)
	{
		m_CurrentAnimation->Stop();

		auto find = m_AnimationList.find(name);

		if (find == m_AnimationList.end())
		{
			GR_CORE_WARN("There is no %s animation", name);
			return;
		}

		m_CurrentAnimation = find->second;
		m_CurrentAnimation->Start();
	}

	void Animator2D::PauseAnimation()
	{
		m_CurrentAnimation->Pause();
	}

	void Animator2D::StopAnimation()
	{
		m_CurrentAnimation->Stop();
	}

	void Animator2D::ResumeAnimation()
	{
		m_CurrentAnimation->Resume();
	}

	void Animator2D::Update(Timestep ts)
	{
		m_CurrentAnimation->Update(ts);
	}
	void Animator2D::Render()
	{
	}
	void Animator2D::Bind(uint32_t slot)
	{
		m_CurrentAnimation->Bind(slot);
	}
}


