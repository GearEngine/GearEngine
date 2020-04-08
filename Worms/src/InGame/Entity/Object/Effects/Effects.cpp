#include "wmpch.h"
#include "Effects.h"

namespace InGame {

	void Blob::reset(const glm::vec3& basePosition, int count)
	{
		m_TextureIndex = 0;
		m_Offset = 0.5f;
		m_TextureIndex = 15;
		m_BasePosition = basePosition;
		m_Position.z = ZOrder::z_Aim + count * 0.001f;
		m_PastTime = 0.0f;
	}

	void Blob::UpDate(float tick)
	{

		m_Position.x = m_BasePosition.x + m_Offset * glm::cos(glm::radians(m_Angle));
		m_Position.y = m_BasePosition.y + m_Offset * glm::sin(glm::radians(m_Angle));
		m_Translate = glm::translate(glm::mat4(1.0f), m_Position) * glm::scale(glm::mat4(1.0f), m_Scale);
		m_Offset += tick * m_OffsetSpeed;

		m_PastTime += tick;
		if (m_PastTime > m_FrameDelay)
		{
			m_PastTime = 0.0f;
			if (m_TextureIndex > 0)
			{
				--m_TextureIndex;
			}
		}
		if (m_Offset > m_LimitOffset)
		{
			m_OnUsing = false;
		}
	}

	void Blob::Render()
	{
  		Gear::Renderer2D::DrawFrameQuad(m_Translate, m_BlobTexture, 0, m_TextureIndex);
	}

	void Blob::SetAngle(float angle)
	{
		m_Angle = angle;
	}

	std::vector<Gear::Ref<Blob>> EffectPool::s_BlobPool = std::vector<Gear::Ref<Blob>>(EFFECT_POOL_MAX);
	int EffectPool::s_BlobPtr = 0;

	void EffectPool::Init()
	{
		for (int i = 0; i < EFFECT_POOL_MAX; ++i)
		{
			s_BlobPool[i].reset(new Blob);
			s_BlobPool[i]->m_BlobTexture = Gear::TextureStorage::GetFrameTexture2D("Blob");
		}
	}

	Gear::Ref<Blob> EffectPool::GetBlob()
	{
		for (s_BlobPtr; s_BlobPtr < EFFECT_POOL_MAX; )
		{
			if (s_BlobPool[s_BlobPtr]->m_OnUsing)
			{
				++s_BlobPtr;
			}
			else
			{
				s_BlobPool[s_BlobPtr]->m_OnUsing = true;
				return s_BlobPool[s_BlobPtr++];
			}
		}
	}


}