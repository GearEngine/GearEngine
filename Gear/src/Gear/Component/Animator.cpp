#include "grpch.h"
#include "Animator.h"

namespace Gear {

	Animator2D::~Animator2D()
	{
		m_CurrentAnimation.reset();
		m_AnimationList.clear();
	}

	void Animator2D::SetCurrentAnimation(EnumType name)
	{
		auto find = m_AnimationList.find(name);
		if (find == m_AnimationList.end())
		{
			GR_CORE_WARN("{0} doesn't exist", name);
			return;
		}
		m_CurrentAnimation = find->second;
	}

	void Animator2D::PlayAnimation(EnumType name)
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
		if (m_CurrentAnimation)
		{
			m_CurrentAnimation->Update(ts);
		}
	}
	void Animator2D::RegisterAnimation(const std::initializer_list<std::pair<const EnumType, Ref<Animation2D>>>& animationList)
	{
		m_AnimationList = animationList;
		m_CurrentAnimation = m_AnimationList.find(2)->second;
	}
}


