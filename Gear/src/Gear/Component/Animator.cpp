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
		m_CurrentAnimation = animation;
	}

	void Animator2D::SetCurrentAnimation(const std::string name)
	{
		auto find = m_AnimationList.find(name);
		if (find == m_AnimationList.end())
		{
			GR_CORE_WARN("{0} doesn't exist", name);
			return;
		}
		m_CurrentAnimation = find->second;
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

	void Animator2D::SetFrameX(int frameX)
	{
		m_CurrentAnimation->Pause();
		m_CurrentAnimation->SetFrameX(frameX);
	}

	void Animator2D::SetFrameY(int frameY)
	{
		m_CurrentAnimation->Pause();
		m_CurrentAnimation->SetFrameY(frameY);
	}

	void Animator2D::Update(Timestep ts)
	{
		m_CurrentAnimation->Update(ts);
	}
}


