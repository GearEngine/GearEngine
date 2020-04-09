#include "wmpch.h"
#include "Effects.h"

namespace InGame {

	void Blob::reset(const glm::vec3& basePosition, int count)
	{
		m_TextureIndex = 0;
		m_Offset = 0.5f;
		m_TextureIndex = 15;
		m_BasePosition = basePosition;
		m_Position.z = ZOrder::z_Blob + count * 0.001f;
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
	
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex25Foom	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex50Foom	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex75Foom	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex100Foom	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex25Biff	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex50Biff	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex75Biff	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex100Biff	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex25Poot	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex50Poot	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex75Poot	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex100Poot	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex25Pow	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex50Pow	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex75Pow	= std::vector<Gear::Ref<ExplosionEffect>>(20);
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex100Pow	= std::vector<Gear::Ref<ExplosionEffect>>(20);

	int EffectPool::s_Ex25FoomPtr	= 0;
	int EffectPool::s_Ex50FoomPtr	= 0;
	int EffectPool::s_Ex75FoomPtr	= 0;
	int EffectPool::s_Ex100FoomPtr	= 0;
	int EffectPool::s_Ex25BiffPtr	= 0;
	int EffectPool::s_Ex50BiffPtr	= 0;
	int EffectPool::s_Ex75BiffPtr	= 0;
	int EffectPool::s_Ex100BiffPtr	= 0;
	int EffectPool::s_Ex25PootPtr	= 0;
	int EffectPool::s_Ex50PootPtr	= 0;
	int EffectPool::s_Ex75PootPtr	= 0;
	int EffectPool::s_Ex100PootPtr	= 0;
	int EffectPool::s_Ex25PowPtr	= 0;
	int EffectPool::s_Ex50PowPtr	= 0;
	int EffectPool::s_Ex75PowPtr	= 0;
	int EffectPool::s_Ex100PowPtr	= 0;

	int EffectPool::s_BlobPtr = 0;

	void EffectPool::Init()
	{
		for (int i = 0; i < EFFECT_POOL_MAX; ++i)
		{
			s_BlobPool[i].reset(new Blob);
			s_BlobPool[i]->m_BlobTexture = Gear::TextureStorage::GetFrameTexture2D("Blob");
		}

		s_Ex25Biff.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex25Biff[i].reset(new ExplosionEffect);
			s_Ex25Biff[i]->init(Explosion::Size25, Explosion::Biff);
		}
		s_Ex50Biff.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex50Biff[i].reset(new ExplosionEffect);
			s_Ex50Biff[i]->init(Explosion::Size50, Explosion::Biff);
		}
		s_Ex75Biff.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex75Biff[i].reset(new ExplosionEffect);
			s_Ex75Biff[i]->init(Explosion::Size75, Explosion::Biff);
		}
		s_Ex100Biff.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex100Biff[i].reset(new ExplosionEffect);
			s_Ex100Biff[i]->init(Explosion::Size100, Explosion::Biff);
		}

		s_Ex25Foom.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex25Foom[i].reset(new ExplosionEffect);
			s_Ex25Foom[i]->init(Explosion::Size25, Explosion::Foom);
		}
		s_Ex50Foom.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex50Foom[i].reset(new ExplosionEffect);
			s_Ex50Foom[i]->init(Explosion::Size50, Explosion::Foom);
		}
		s_Ex75Foom.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex75Foom[i].reset(new ExplosionEffect);
			s_Ex75Foom[i]->init(Explosion::Size75, Explosion::Foom);
		}
		s_Ex100Foom.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex100Foom[i].reset(new ExplosionEffect);
			s_Ex100Foom[i]->init(Explosion::Size100, Explosion::Foom);
		}

		s_Ex25Poot.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex25Poot[i].reset(new ExplosionEffect);
			s_Ex25Poot[i]->init(Explosion::Size25, Explosion::Poot);
		}
		s_Ex50Poot.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex50Poot[i].reset(new ExplosionEffect);
			s_Ex50Poot[i]->init(Explosion::Size50, Explosion::Poot);
		}
		s_Ex75Poot.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex75Poot[i].reset(new ExplosionEffect);
			s_Ex75Poot[i]->init(Explosion::Size75, Explosion::Poot);
		}
		s_Ex100Poot.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex100Poot[i].reset(new ExplosionEffect);
			s_Ex100Poot[i]->init(Explosion::Size100, Explosion::Poot);
		}

		s_Ex25Pow.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex25Pow[i].reset(new ExplosionEffect);
			s_Ex25Pow[i]->init(Explosion::Size25, Explosion::Pow);
		}
		s_Ex50Pow.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex50Pow[i].reset(new ExplosionEffect);
			s_Ex50Pow[i]->init(Explosion::Size50, Explosion::Pow);
		}
		s_Ex75Pow.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex75Pow[i].reset(new ExplosionEffect);
			s_Ex75Pow[i]->init(Explosion::Size75, Explosion::Pow);
		}
		s_Ex100Pow.resize(10);
		for (int i = 0; i < 10; ++i)
		{
			s_Ex100Pow[i].reset(new ExplosionEffect);
			s_Ex100Pow[i]->init(Explosion::Size100, Explosion::Pow);
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

	Gear::Ref<ExplosionEffect> EffectPool::GetExplosion(Explosion::Size size, Explosion::Text text)
	{
		switch (size)
		{
		case InGame::Explosion::Size25:
			switch (text)
			{
			case InGame::Explosion::Foom:
				while (1)
				{
					if (s_Ex25Foom[s_Ex25FoomPtr]->m_OnUsing)
					{
						++s_Ex25FoomPtr;
						if (s_Ex25FoomPtr >= 10)
						{
							s_Ex25FoomPtr = 0;
						}
					}
					else
					{
						return s_Ex25Foom[s_Ex25FoomPtr];
					}
				}
			case InGame::Explosion::Biff:
				while (1)
				{
					if (s_Ex25Biff[s_Ex25BiffPtr]->m_OnUsing)
					{
						++s_Ex25BiffPtr;
						if (s_Ex25BiffPtr >= 10)
						{
							s_Ex25BiffPtr = 0;
						}
					}
					else
					{
						return s_Ex25Biff[s_Ex25BiffPtr];
					}
				}
			case InGame::Explosion::Poot:
				while (1)
				{
					if (s_Ex25Poot[s_Ex25PootPtr]->m_OnUsing)
					{
						++s_Ex25PootPtr;
						if (s_Ex25PootPtr >= 10)
						{
							s_Ex25PootPtr = 0;
						}
					}
					else
					{
						return s_Ex25Poot[s_Ex25PootPtr];
					}
				}
			case InGame::Explosion::Pow:
				while (1)
				{
					if (s_Ex25Pow[s_Ex25PowPtr]->m_OnUsing)
					{
						++s_Ex25PowPtr;
						if (s_Ex25PowPtr >= 10)
						{
							s_Ex25PowPtr = 0;
						}
					}
					else
					{
						return s_Ex25Pow[s_Ex25PowPtr];
					}
				}
			}
		case InGame::Explosion::Size50:
			switch (text)
			{
			case InGame::Explosion::Foom:
				while (1)
				{
					if (s_Ex50Foom[s_Ex50FoomPtr]->m_OnUsing)
					{
						++s_Ex50FoomPtr;
						if (s_Ex50FoomPtr >= 10)
						{
							s_Ex50FoomPtr = 0;
						}
					}
					else
					{
						return s_Ex50Foom[s_Ex50FoomPtr];
					}
				}
			case InGame::Explosion::Biff:
				while (1)
				{
					if (s_Ex50Biff[s_Ex50BiffPtr]->m_OnUsing)
					{
						++s_Ex50BiffPtr;
						if (s_Ex50BiffPtr >= 10)
						{
							s_Ex50BiffPtr = 0;
						}
					}
					else
					{
						return s_Ex50Biff[s_Ex50BiffPtr];
					}
				}
			case InGame::Explosion::Poot:
				while (1)
				{
					if (s_Ex50Poot[s_Ex50PootPtr]->m_OnUsing)
					{
						++s_Ex50PootPtr;
						if (s_Ex50PootPtr >= 10)
						{
							s_Ex50PootPtr = 0;
						}
					}
					else
					{
						return s_Ex50Poot[s_Ex50PootPtr];
					}
				}
			case InGame::Explosion::Pow:
				while (1)
				{
					if (s_Ex50Pow[s_Ex50PowPtr]->m_OnUsing)
					{
						++s_Ex50PowPtr;
						if (s_Ex50PowPtr >= 10)
						{
							s_Ex50PowPtr = 0;
						}
					}
					else
					{
						return s_Ex50Pow[s_Ex50PowPtr];
					}
				}
			}
		case InGame::Explosion::Size75:
			switch (text)
			{
			case InGame::Explosion::Foom:
				while (1)
				{
					if (s_Ex75Foom[s_Ex75FoomPtr]->m_OnUsing)
					{
						++s_Ex75FoomPtr;
						if (s_Ex75FoomPtr >= 10)
						{
							s_Ex75FoomPtr = 0;
						}
					}
					else
					{
						return s_Ex75Foom[s_Ex75FoomPtr];
					}
				}
			case InGame::Explosion::Biff:
				while (1)
				{
					if (s_Ex75Biff[s_Ex75BiffPtr]->m_OnUsing)
					{
						++s_Ex75BiffPtr;
						if (s_Ex75BiffPtr >= 10)
						{
							s_Ex75BiffPtr = 0;
						}
					}
					else
					{
						return s_Ex75Biff[s_Ex75BiffPtr];
					}
				}
			case InGame::Explosion::Poot:
				while (1)
				{
					if (s_Ex75Poot[s_Ex75PootPtr]->m_OnUsing)
					{
						++s_Ex75PootPtr;
						if (s_Ex75PootPtr >= 10)
						{
							s_Ex75PootPtr = 0;
						}
					}
					else
					{
						return s_Ex75Poot[s_Ex75PootPtr];
					}
				}
			case InGame::Explosion::Pow:
				while (1)
				{
					if (s_Ex75Pow[s_Ex75PowPtr]->m_OnUsing)
					{
						++s_Ex75PowPtr;
						if (s_Ex75PowPtr >= 10)
						{
							s_Ex75PowPtr = 0;
						}
					}
					else
					{
						return s_Ex75Pow[s_Ex75PowPtr];
					}
				}
			}
		case InGame::Explosion::Size100:
			switch (text)
			{
			case InGame::Explosion::Foom:
				while (1)
				{
					if (s_Ex100Foom[s_Ex100FoomPtr]->m_OnUsing)
					{
						++s_Ex100FoomPtr;
						if (s_Ex100FoomPtr >= 10)
						{
							s_Ex100FoomPtr = 0;
						}
					}
					else
					{
						return s_Ex100Foom[s_Ex100FoomPtr];
					}
				}
			case InGame::Explosion::Biff:
				while (1)
				{
					if (s_Ex100Biff[s_Ex100BiffPtr]->m_OnUsing)
					{
						++s_Ex100BiffPtr;
						if (s_Ex100BiffPtr >= 10)
						{
							s_Ex100BiffPtr = 0;
						}
					}
					else
					{
						return s_Ex100Biff[s_Ex100BiffPtr];
					}
				}
			case InGame::Explosion::Poot:
				while (1)
				{
					if (s_Ex100Poot[s_Ex100PootPtr]->m_OnUsing)
					{
						++s_Ex100PootPtr;
						if (s_Ex100PootPtr >= 10)
						{
							s_Ex100PootPtr = 0;
						}
					}
					else
					{
						return s_Ex100Poot[s_Ex100PootPtr];
					}
				}
			case InGame::Explosion::Pow:
				while (1)
				{
					if (s_Ex100Pow[s_Ex100PowPtr]->m_OnUsing)
					{
						++s_Ex100PowPtr;
						if (s_Ex100PowPtr >= 10)
						{
							s_Ex100PowPtr = 0;
						}
					}
					else
					{
						return s_Ex100Pow[s_Ex100PowPtr];
					}
				}
			}
		}
	}
	

	void ExplosionEffect::init(Explosion::Size size, Explosion::Text explosionText)
	{
		switch (explosionText)
		{
		case InGame::Explosion::Foom:
			m_Text = Gear::TextureStorage::GetFrameTexture2D("ExplosionTextFoom");
			m_StartTextIndex = 19;
			break;
		case InGame::Explosion::Biff:
			m_Text = Gear::TextureStorage::GetFrameTexture2D("ExplosionTextBiff");
			m_StartTextIndex = 11;
			break;
		case InGame::Explosion::Poot:
			m_Text = Gear::TextureStorage::GetFrameTexture2D("ExplosionTextPoot");
			m_StartTextIndex = 17;
			break;
		case InGame::Explosion::Pow:
			m_Text = Gear::TextureStorage::GetFrameTexture2D("ExplosionTextPow");
			m_StartTextIndex = 11;
			break;
		}

		switch (size)
		{
		case InGame::Explosion::Size25:
			m_Circle = Gear::TextureStorage::GetFrameTexture2D("ExplosionCircle25");
			m_Ellipse = Gear::TextureStorage::GetFrameTexture2D("ExplosionEllipse25");
			m_StartCircleIndex = 7;
			m_StartEllipseIndex = 19;
			break;
		case InGame::Explosion::Size50:
			m_Circle = Gear::TextureStorage::GetFrameTexture2D("ExplosionCircle50");
			m_Ellipse = Gear::TextureStorage::GetFrameTexture2D("ExplosionEllipse50");
			m_StartCircleIndex = 7;
			m_StartEllipseIndex = 19;
			break;
		case InGame::Explosion::Size75:
			m_Circle = Gear::TextureStorage::GetFrameTexture2D("ExplosionCircle75");
			m_Ellipse = Gear::TextureStorage::GetFrameTexture2D("ExplosionEllipse75");
			m_StartCircleIndex = 3;
			m_StartEllipseIndex = 9;
			break;
		case InGame::Explosion::Size100:
			m_Circle = Gear::TextureStorage::GetFrameTexture2D("ExplosionCircle100");
			m_Ellipse = Gear::TextureStorage::GetFrameTexture2D("ExplosionEllipse100");
			m_StartCircleIndex = 3;
			m_StartEllipseIndex = 9;
			break;
		}

		float circleWidth = m_Circle->GetWidth();
		float circleHeight = m_Circle->GetHeight();
		m_CircleScale = glm::vec3(circleWidth / 60.0f * 1.8f, circleHeight / 60.0f * 1.8f, 1.0f);

		float ellipseWidth = m_Ellipse->GetWidth();
		float ellipseHeight = m_Ellipse->GetHeight();
		m_EllipseScale = glm::vec3(ellipseWidth / 60.0f * 1.8f, ellipseHeight / 60.0f * 1.8f, 1.0f);

		float textWidth = m_Text->GetWidth();
		float textHeight = m_Text->GetHeight();
		m_TextScale = glm::vec3(textWidth / 60.0f * 1.8f, textHeight / 60.0f * 1.8f, 1.0f);

	}

	void ExplosionEffect::init(const glm::vec2 & basePosition)
	{
		m_InFisrt = true;
		m_OnUsing = true;

		m_CircleIndex = m_StartCircleIndex;
		m_TextIndex = m_StartTextIndex;
		m_EllipseIndex = m_StartEllipseIndex;

		m_CircleTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(basePosition, ZOrder::z_ExplosionCircle))* glm::scale(glm::mat4(1.0f), m_CircleScale);
		m_EllipseTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(basePosition, ZOrder::z_ExplosionEllipse))* glm::scale(glm::mat4(1.0f), m_EllipseScale);
		m_TextTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(basePosition, ZOrder::z_ExplosionText))* glm::scale(glm::mat4(1.0f), m_TextScale);
	}

	void ExplosionEffect::Update(float ts)
	{
		if (m_InFisrt)
		{
			m_InFisrt = false;
			return;
		}
		if (m_CircleIndex < 0 && m_EllipseIndex < 0 && m_TextIndex < 0)
		{
			m_OnUsing = false;
			m_InFisrt = true;
			return;
		}

		m_pastTime += ts;
		if (m_FrameDelay < m_pastTime)
		{
			m_pastTime = 0.0f;
			--m_CircleIndex;
			--m_EllipseIndex;
			--m_TextIndex;
		}
	}

	void ExplosionEffect::Render()
	{
		if (m_CircleIndex >= 0)
		{
			Gear::Renderer2D::DrawFrameQuad(m_CircleTranslate, m_Circle, 0, m_CircleIndex);
		}
		if (m_EllipseIndex >= 0)
		{
			Gear::Renderer2D::DrawFrameQuad(m_EllipseTranslate, m_Ellipse, 0, m_EllipseIndex);
		}
		if (m_TextIndex >= 0)
		{
			Gear::Renderer2D::DrawFrameQuad(m_TextTranslate, m_Text, 0, m_TextIndex);
		}
	}

}