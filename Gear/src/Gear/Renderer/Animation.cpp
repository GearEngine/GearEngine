#include "grpch.h"
#include "Animation.h"

namespace Gear {
	Animation2D::Animation2D(Ref<FrameTexture2D> sprite, float fps, bool loop)
		:m_Sprite(sprite), m_Fps(fps), m_Loop(loop)
	{
		m_CurrentFrameX = 0;
		m_CurrentFrameY = 0;
		m_MaxFrameX = sprite->GetMaxFrameX();
		m_MaxFrameY = sprite->GetMaxFrameY();
	}

	Animation2D::Animation2D(Ref<FrameTexture2D> sprite, float fps, const std::vector<std::pair<int, int>> frameOrder, bool loop)
		:m_Sprite(sprite), m_Fps(fps), m_FrameOrder(frameOrder), m_Loop(loop)
	{
		m_HasFrameOrder = true;
		m_CurrentFrameX = 0;
		m_CurrentFrameY = 0;
		m_MaxFrameX = sprite->GetMaxFrameX();
		m_MaxFrameY = sprite->GetMaxFrameY();
	}

	Ref<Animation2D> Animation2D::Create(Ref<FrameTexture2D> sprite, float fps, bool loop)
	{
		return CreateRef<Animation2D>(sprite, fps, loop);
	}

	Ref<Animation2D> Animation2D::Create(Ref<FrameTexture2D> sprite, float fps, const std::vector<std::pair<int, int>> frameOrder, bool loop)
	{
		return CreateRef<Animation2D>(sprite, fps, frameOrder, loop);
	}

	void Animation2D::Update(Timestep ts)
	{
		if (!m_OnAnimation)
			return;

		pastTime += ts;
		if (m_Fps <= pastTime)
		{
			pastTime = 0.0f;
			if (m_HasFrameOrder)
			{
				++m_FrameOrderIndex;
				if (m_FrameOrderIndex > m_FrameOrder.size())
				{
					if (m_Loop)
					{
						m_FrameOrderIndex = 0;
					}
					else
					{
						m_OnAnimation = false;
						return;
					}
				}
				auto[m_CurrentFrameX, m_CurrentFrameY] = m_FrameOrder[m_FrameOrderIndex];
			}
			else
			{
				++m_CurrentFrameY;
				if (m_CurrentFrameY >= m_MaxFrameY && m_CurrentFrameX < m_MaxFrameX - 1)
				{
					++m_CurrentFrameX;
					m_CurrentFrameY = 0;
				}
				else if(m_CurrentFrameY >= m_MaxFrameY && m_CurrentFrameX >= m_MaxFrameX - 1)
				{
					if (m_Loop)
					{
						m_CurrentFrameX = 0;
						m_CurrentFrameY = 0;
					}
					else
					{
						--m_CurrentFrameY;
						m_OnAnimation = false;
						return;
					}
				}
			}
		}
	}

	void Animation2D::Bind(uint32_t slot) const
	{
		m_Sprite->Bind(m_CurrentFrameX, m_CurrentFrameY, slot);
	}


	void Animation2D::Start()
	{
		if (m_HasFrameOrder)
		{
			m_FrameOrderIndex = 0;
		}
		else
		{
			m_CurrentFrameX = 0;
			m_CurrentFrameY = 0;
		}
		m_OnAnimation = true;
	}

	void Animation2D::Stop()
	{
		if (m_HasFrameOrder)
		{
			m_FrameOrderIndex = 0;
		}
		else
		{
			m_CurrentFrameX = 0;
			m_CurrentFrameY = 0;
		}
		m_OnAnimation = false;
	}

	void Animation2D::Resume()
	{
		m_OnAnimation = false;
	}

	void Animation2D::Pause()
	{
		m_OnAnimation = true;
	}
}