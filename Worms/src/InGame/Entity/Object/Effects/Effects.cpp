#include "wmpch.h"
#include "Effects.h"
#include "InGame/Layer/ObjectLayer.h"

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
	std::vector<Gear::Ref<Spangle>> EffectPool::s_Spangle = std::vector<Gear::Ref<Spangle>>();
	std::vector<Gear::Ref<Marker>> EffectPool::s_Marker = std::vector<Gear::Ref<Marker>>();

	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex25 = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex50Foom = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex75Foom = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex100Foom = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex50Biff = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex75Biff = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex100Biff = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex50Poot = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex75Poot = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex100Poot = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex50Pow = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex75Pow = std::vector<Gear::Ref<ExplosionEffect>>();
	std::vector<Gear::Ref<ExplosionEffect>> EffectPool::s_Ex100Pow = std::vector<Gear::Ref<ExplosionEffect>>();

	std::vector<Gear::Ref<ExplosionSmokeBunddle>> EffectPool::s_Sm25 = std::vector<Gear::Ref<ExplosionSmokeBunddle>>();
	std::vector<Gear::Ref<ExplosionSmokeBunddle>> EffectPool::s_Sm50 = std::vector<Gear::Ref<ExplosionSmokeBunddle>>();
	std::vector<Gear::Ref<ExplosionSmokeBunddle>> EffectPool::s_Sm75 = std::vector<Gear::Ref<ExplosionSmokeBunddle>>();
	std::vector<Gear::Ref<ExplosionSmokeBunddle>> EffectPool::s_Sm100 = std::vector<Gear::Ref<ExplosionSmokeBunddle>>();

	std::vector<Gear::Ref<FlameBundle>> EffectPool::s_Fm25 = std::vector<Gear::Ref<FlameBundle>>();
	std::vector<Gear::Ref<FlameBundle>> EffectPool::s_Fm50 = std::vector<Gear::Ref<FlameBundle>>();
	std::vector<Gear::Ref<FlameBundle>> EffectPool::s_Fm75 = std::vector<Gear::Ref<FlameBundle>>();
	std::vector<Gear::Ref<FlameBundle>> EffectPool::s_Fm100 = std::vector<Gear::Ref<FlameBundle>>();

	std::vector<Gear::Ref<Smoke>> EffectPool::s_Sm20 = std::vector<Gear::Ref<Smoke>>();
	std::vector<Gear::Ref<Smoke>> EffectPool::s_Sm30 = std::vector<Gear::Ref<Smoke>>();
	std::vector<Gear::Ref<Smoke>> EffectPool::s_Sm40 = std::vector<Gear::Ref<Smoke>>();

	std::vector<Gear::Ref<Exhaust>> EffectPool::s_Exhaust1 = std::vector<Gear::Ref<Exhaust>>();
	std::vector<Gear::Ref<Exhaust>> EffectPool::s_ExhaustMagic = std::vector<Gear::Ref<Exhaust>>();

	int EffectPool::explosionMax = 30;
	int EffectPool::flameMax = 30;

	int EffectPool::s_Ex25Ptr = 0;
	int EffectPool::s_Ex50FoomPtr = 0;
	int EffectPool::s_Ex75FoomPtr = 0;
	int EffectPool::s_Ex100FoomPtr = 0;
	int EffectPool::s_Ex50BiffPtr = 0;
	int EffectPool::s_Ex75BiffPtr = 0;
	int EffectPool::s_Ex100BiffPtr = 0;
	int EffectPool::s_Ex50PootPtr = 0;
	int EffectPool::s_Ex75PootPtr = 0;
	int EffectPool::s_Ex100PootPtr = 0;
	int EffectPool::s_Ex50PowPtr = 0;
	int EffectPool::s_Ex75PowPtr = 0;
	int EffectPool::s_Ex100PowPtr = 0;

	int EffectPool::s_Sm25Ptr = 0;
	int EffectPool::s_Sm50Ptr = 0;
	int EffectPool::s_Sm75Ptr = 0;
	int EffectPool::s_Sm100Ptr = 0;

	int EffectPool::s_Fm25Ptr = 0;
	int EffectPool::s_Fm50Ptr = 0;
	int EffectPool::s_Fm75Ptr = 0;
	int EffectPool::s_Fm100Ptr = 0;

	int EffectPool::s_Sm20Ptr = 0;
	int EffectPool::s_Sm30Ptr = 0;
	int EffectPool::s_Sm40Ptr = 0;

	int EffectPool::s_Exhaust1Ptr = 0;
	int EffectPool::s_ExhaustMagicPtr = 0;

	int EffectPool::s_BlobPtr = 0;

	void EffectPool::Init()
	{
		for (int i = 0; i < EFFECT_POOL_MAX; ++i)
		{
			s_BlobPool[i].reset(new Blob);
			s_BlobPool[i]->m_BlobTexture = Gear::TextureStorage::GetFrameTexture2D("Blob");
		}

		//spangle
		s_Spangle.resize(6);
		for (int i = TeamColor::Red; i <= TeamColor::Sky; ++i)
		{
			s_Spangle[i - TeamColor::Red].reset(new Spangle);
			s_Spangle[i - TeamColor::Red]->init((TeamColor::Color)i);
		}

		//Marker
		s_Marker.resize(6);
		for (int i = TeamColor::Red; i <= TeamColor::Sky; ++i)
		{
			s_Marker[i - TeamColor::Red].reset(new Marker);
			s_Marker[i - TeamColor::Red]->init((TeamColor::Color)i);
		}

		//Explosion
		
		s_Ex25.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex25[i].reset(new ExplosionEffect);
			s_Ex25[i]->init(Explosion::Size25, Explosion::Null);
		}
		s_Ex50Biff.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex50Biff[i].reset(new ExplosionEffect);
			s_Ex50Biff[i]->init(Explosion::Size50, Explosion::Biff);
		}
		s_Ex75Biff.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex75Biff[i].reset(new ExplosionEffect);
			s_Ex75Biff[i]->init(Explosion::Size75, Explosion::Biff);
		}
		s_Ex100Biff.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex100Biff[i].reset(new ExplosionEffect);
			s_Ex100Biff[i]->init(Explosion::Size100, Explosion::Biff);
		}


		s_Ex50Foom.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex50Foom[i].reset(new ExplosionEffect);
			s_Ex50Foom[i]->init(Explosion::Size50, Explosion::Foom);
		}
		s_Ex75Foom.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex75Foom[i].reset(new ExplosionEffect);
			s_Ex75Foom[i]->init(Explosion::Size75, Explosion::Foom);
		}
		s_Ex100Foom.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex100Foom[i].reset(new ExplosionEffect);
			s_Ex100Foom[i]->init(Explosion::Size100, Explosion::Foom);
		}


		s_Ex50Poot.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex50Poot[i].reset(new ExplosionEffect);
			s_Ex50Poot[i]->init(Explosion::Size50, Explosion::Poot);
		}
		s_Ex75Poot.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex75Poot[i].reset(new ExplosionEffect);
			s_Ex75Poot[i]->init(Explosion::Size75, Explosion::Poot);
		}
		s_Ex100Poot.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex100Poot[i].reset(new ExplosionEffect);
			s_Ex100Poot[i]->init(Explosion::Size100, Explosion::Poot);
		}

		s_Ex50Pow.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex50Pow[i].reset(new ExplosionEffect);
			s_Ex50Pow[i]->init(Explosion::Size50, Explosion::Pow);
		}
		s_Ex75Pow.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex75Pow[i].reset(new ExplosionEffect);
			s_Ex75Pow[i]->init(Explosion::Size75, Explosion::Pow);
		}
		s_Ex100Pow.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Ex100Pow[i].reset(new ExplosionEffect);
			s_Ex100Pow[i]->init(Explosion::Size100, Explosion::Pow);
		}

		//Smoke
		s_Sm25.resize(30);
		for (int i = 0; i < 30; ++i)
		{
			s_Sm25[i].reset(new ExplosionSmokeBunddle);
			s_Sm25[i]->init(Explosion::Size::Size25);
		}
		s_Sm50.resize(30);
		for (int i = 0; i < 30; ++i)
		{
			s_Sm50[i].reset(new ExplosionSmokeBunddle);
			s_Sm50[i]->init(Explosion::Size::Size50);
		}
		s_Sm75.resize(30);
		for (int i = 0; i < 30; ++i)
		{
			s_Sm75[i].reset(new ExplosionSmokeBunddle);
			s_Sm75[i]->init(Explosion::Size::Size75);
		}
		s_Sm100.resize(30);
		for (int i = 0; i < 30; ++i)
		{
			s_Sm100[i].reset(new ExplosionSmokeBunddle);
			s_Sm100[i]->init(Explosion::Size::Size100);
		}

		//Flame
		s_Fm25.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Fm25[i].reset(new FlameBundle);
			s_Fm25[i]->init(Explosion::Size::Size25);
		}
		s_Fm50.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Fm50[i].reset(new FlameBundle);
			s_Fm50[i]->init(Explosion::Size::Size50);
		}
		s_Fm75.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Fm75[i].reset(new FlameBundle);
			s_Fm75[i]->init(Explosion::Size::Size75);
		}
		s_Fm100.resize(explosionMax);
		for (int i = 0; i < explosionMax; ++i)
		{
			s_Fm100[i].reset(new FlameBundle);
			s_Fm100[i]->init(Explosion::Size::Size100);
		}

		s_Sm20.resize(150);
		for (int i = 0; i < 150; ++i)
		{
			s_Sm20[i].reset(new Smoke);
			s_Sm20[i]->init(Explosion::SmokeSize::Size20);
		}
		s_Sm30.resize(150);
		for (int i = 0; i < 150; ++i)
		{
			s_Sm30[i].reset(new Smoke);
			s_Sm20[i]->init(Explosion::SmokeSize::Size30);
		}
		s_Sm40.resize(150);
		for (int i = 0; i < 150; ++i)
		{
			s_Sm40[i].reset(new Smoke);
			s_Sm20[i]->init(Explosion::SmokeSize::Size40);
		}

		//Exhaust
		s_Exhaust1.resize(50);
		for (int i = 0; i < 50; ++i)
		{
			s_Exhaust1[i].reset(new Exhaust);
			s_Exhaust1[i]->init(ExhaustType::_1);
		}
		s_ExhaustMagic.resize(EFFECT_POOL_MAX);
		for (int i = 0; i < EFFECT_POOL_MAX; ++i)
		{
			s_ExhaustMagic[i].reset(new Exhaust);
			s_ExhaustMagic[i]->init(ExhaustType::Magic);
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
			while (1)
			{
				if (s_Ex25[s_Ex25Ptr]->m_OnUsing)
				{
					++s_Ex25Ptr;
					if (s_Ex25Ptr >= explosionMax)
					{
						s_Ex25Ptr = 0;
					}
				}
				else
				{
					return s_Ex25[s_Ex25Ptr];
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
						if (s_Ex50FoomPtr >= explosionMax)
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
						if (s_Ex50BiffPtr >= explosionMax)
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
						if (s_Ex50PootPtr >= explosionMax)
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
						if (s_Ex50PowPtr >= explosionMax)
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
						if (s_Ex75FoomPtr >= explosionMax)
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
						if (s_Ex75BiffPtr >= explosionMax)
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
						if (s_Ex75PootPtr >= explosionMax)
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
						if (s_Ex75PowPtr >= explosionMax)
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
						if (s_Ex100FoomPtr >= explosionMax)
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
						if (s_Ex100BiffPtr >= explosionMax)
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
						if (s_Ex100PootPtr >= explosionMax)
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
						if (s_Ex100PowPtr >= explosionMax)
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

	Gear::Ref<ExplosionSmokeBunddle> EffectPool::GetExplosionSmoke(Explosion::Size size)
	{
		switch (size)
		{
		case InGame::Explosion::Size25:
			while (1)
			{
				++s_Sm25Ptr;
				if (s_Sm25Ptr >= 30)
				{
					s_Sm25Ptr = 0;
				}
				if (!s_Sm25[s_Sm25Ptr]->m_OnUsing)
				{
					return s_Sm25[s_Sm25Ptr];
				}
			}
		case InGame::Explosion::Size50:
			while (1)
			{
				++s_Sm50Ptr;
				if (s_Sm50Ptr >= 30)
				{
					s_Sm50Ptr = 0;
				}
				if (!s_Sm50[s_Sm50Ptr]->m_OnUsing)
				{
					return s_Sm50[s_Sm50Ptr];
				}
			}
		case InGame::Explosion::Size75:
			while (1)
			{
				++s_Sm75Ptr;
				if (s_Sm75Ptr >= 30)
				{
					s_Sm75Ptr = 0;
				}
				if (!s_Sm75[s_Sm75Ptr]->m_OnUsing)
				{
					return s_Sm75[s_Sm75Ptr];
				}
			}
		case InGame::Explosion::Size100:
			while (1)
			{
				++s_Sm100Ptr;
				if (s_Sm100Ptr >= 30)
				{
					s_Sm100Ptr = 0;
				}
				if (!s_Sm100[s_Sm100Ptr]->m_OnUsing)
				{
					return s_Sm100[s_Sm100Ptr];
				}
			}
		}
		return nullptr;
	}

	Gear::Ref<FlameBundle> EffectPool::GetFlame(Explosion::Size size)
	{
		switch (size)
		{
		case InGame::Explosion::Size25:
			while (1)
			{
				++s_Fm25Ptr;
				if (s_Fm25Ptr >= explosionMax)
				{
					s_Fm25Ptr = 0;
				}
				if (!s_Fm25[s_Fm25Ptr]->m_OnUsing)
				{
					return s_Fm25[s_Fm25Ptr];
				}
			}
			break;
		case InGame::Explosion::Size50:
			while (1)
			{
				++s_Fm50Ptr;
				if (s_Fm50Ptr >= explosionMax)
				{
					s_Fm50Ptr = 0;
				}
				if (!s_Fm50[s_Fm50Ptr]->m_OnUsing)
				{
					return s_Fm50[s_Fm50Ptr];
				}
			}
			break;
		case InGame::Explosion::Size75:
			while (1)
			{
				++s_Fm75Ptr;
				if (s_Fm75Ptr >= explosionMax)
				{
					s_Fm75Ptr = 0;
				}
				if (!s_Fm75[s_Fm75Ptr]->m_OnUsing)
				{
					return s_Fm75[s_Fm75Ptr];
				}
			}
			break;
		case InGame::Explosion::Size100:
			while (1)
			{
				++s_Fm100Ptr;
				if (s_Fm100Ptr >= explosionMax)
				{
					s_Fm100Ptr = 0;
				}
				if (!s_Fm100[s_Fm100Ptr]->m_OnUsing)
				{
					return s_Fm100[s_Fm100Ptr];
				}
			}
			break;
		}
		return nullptr;
	}

	Gear::Ref<Smoke> EffectPool::GetSmoke(Explosion::SmokeSize size)
	{
		switch (size)
		{
		case InGame::Explosion::Size20:
			while (1)
			{
				++s_Sm20Ptr;
				if (s_Sm20Ptr >= 150)
				{
					s_Sm20Ptr = 0;
				}
				if (!s_Sm20[s_Sm20Ptr]->m_OnUsing)
				{
					return s_Sm20[s_Sm20Ptr];
				}
			}
			break;
		case InGame::Explosion::Size30:
			while (1)
			{
				++s_Sm30Ptr;
				if (s_Sm30Ptr >= 150)
				{
					s_Sm30Ptr = 0;
				}
				if (!s_Sm30[s_Sm30Ptr]->m_OnUsing)
				{
					return s_Sm30[s_Sm30Ptr];
				}
			}
			break;
		case InGame::Explosion::Size40:
			while (1)
			{
				++s_Sm40Ptr;
				if (s_Sm40Ptr >= 150)
				{
					s_Sm40Ptr = 0;
				}
				if (!s_Sm40[s_Sm40Ptr]->m_OnUsing)
				{
					return s_Sm40[s_Sm40Ptr];
				}
			}
			break;
		}
		return nullptr;
	}

	Gear::Ref<Exhaust> EffectPool::GetExhuast(ExhaustType::Type type)
	{
		switch (type)
		{
		case InGame::ExhaustType::_1:
			while (1)
			{
				++s_Exhaust1Ptr;
				if (s_Exhaust1Ptr >= 50)
				{
					s_Exhaust1Ptr = 0;
				}
				if (!s_Exhaust1[s_Exhaust1Ptr]->m_OnUsing)
				{
					return s_Exhaust1[s_Exhaust1Ptr];
				}
			}
		case InGame::ExhaustType::Magic:
			while (1)
			{
				++s_ExhaustMagicPtr;
				if (s_ExhaustMagicPtr >= EFFECT_POOL_MAX)
				{
					s_ExhaustMagicPtr = 0;
				}
				if (!s_ExhaustMagic[s_ExhaustMagicPtr]->m_OnUsing)
				{
					return s_ExhaustMagic[s_ExhaustMagicPtr];
				}
			}
		}
		return nullptr;
	}

	Gear::Ref<Spangle> EffectPool::GetSpangle(TeamColor::Color color)
	{
		switch (color)
		{
		case InGame::TeamColor::Red:
			return s_Spangle[0];
		case InGame::TeamColor::Blue:
			return s_Spangle[1];
		case InGame::TeamColor::Green:
			return s_Spangle[2];
		case InGame::TeamColor::Yellow:
			return s_Spangle[3];
		case InGame::TeamColor::Purple:
			return s_Spangle[4];
		case InGame::TeamColor::Sky:
			return s_Spangle[5];
		}
		return nullptr;
	}

	Gear::Ref<Marker> EffectPool::GetMarker(TeamColor::Color color)
	{
		switch (color)
		{
		case InGame::TeamColor::Red:
			return s_Marker[0];
		case InGame::TeamColor::Blue:
			return s_Marker[1];
		case InGame::TeamColor::Green:
			return s_Marker[2];
		case InGame::TeamColor::Yellow:
			return s_Marker[3];
		case InGame::TeamColor::Purple:
			return s_Marker[4];
		case InGame::TeamColor::Sky:
			return s_Marker[5];
		}
		return nullptr;
	}

	void EffectPool::pushExplosion(Gear::Ref<ExplosionEffect> explosion)
	{
		ObjectLayer::s_Explosion.push_back(explosion);
	}

	void EffectPool::pushExplosionSmoke(Gear::Ref<ExplosionSmokeBunddle> explosionSmoke)
	{
		ObjectLayer::s_ExplosionSmoke.push_back(explosionSmoke);
	}

	void EffectPool::pushFlame(Gear::Ref<FlameBundle> flame)
	{
		ObjectLayer::s_Flames.push_back(flame);
	}

	void EffectPool::pushSpangle(Gear::Ref<Spangle> spangle)
	{
		ObjectLayer::s_Spangle.push_back(spangle);
	}

	void EffectPool::pushMarker(Gear::Ref<Marker> marker)
	{
		ObjectLayer::s_Marker.push_back(marker);
	}

	void EffectPool::pushExhaust(Gear::Ref<Exhaust> exhaust)
	{
		ObjectLayer::s_Exhausts.push_back(exhaust);
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
		case InGame::Explosion::Null:
			m_StartTextIndex = -1;
			break;
		}

		switch (size)
		{
		case InGame::Explosion::Size25:
			m_Circle = Gear::TextureStorage::GetFrameTexture2D("ExplosionCircle25");
			m_Ellipse = Gear::TextureStorage::GetFrameTexture2D("ExplosionEllipse25");
			m_StartCircleIndex = 7;
			m_StartEllipseIndex = 19;
			m_FrameDelay = 0.02f;
			break;
		case InGame::Explosion::Size50:
			m_Circle = Gear::TextureStorage::GetFrameTexture2D("ExplosionCircle50");
			m_Ellipse = Gear::TextureStorage::GetFrameTexture2D("ExplosionEllipse50");
			m_StartCircleIndex = 7;
			m_StartEllipseIndex = 19;
			m_FrameDelay = 0.02f;
			break;
		case InGame::Explosion::Size75:
			m_Circle = Gear::TextureStorage::GetFrameTexture2D("ExplosionCircle75");
			m_Ellipse = Gear::TextureStorage::GetFrameTexture2D("ExplosionEllipse75");
			m_StartCircleIndex = 3;
			m_StartEllipseIndex = 9;
			m_FrameDelay = 0.04f;
			break;
		case InGame::Explosion::Size100:
			m_Circle = Gear::TextureStorage::GetFrameTexture2D("ExplosionCircle100");
			m_Ellipse = Gear::TextureStorage::GetFrameTexture2D("ExplosionEllipse100");
			m_StartCircleIndex = 3;
			m_StartEllipseIndex = 9;
			m_FrameDelay = 0.04f;
			break;
		}

		float circleWidth = m_Circle->GetWidth();
		float circleHeight = m_Circle->GetHeight();
		m_CircleScale = glm::vec3(circleWidth / 60.0f * 1.8f, circleHeight / 60.0f * 1.8f, 1.0f);

		float ellipseWidth = m_Ellipse->GetWidth();
		float ellipseHeight = m_Ellipse->GetHeight();
		m_EllipseScale = glm::vec3(ellipseWidth / 60.0f * 1.8f, ellipseHeight / 60.0f * 1.8f, 1.0f);

		if (m_Text)
		{
			float textWidth = m_Text->GetWidth();
			float textHeight = m_Text->GetHeight();
			m_TextScale = glm::vec3(textWidth / 60.0f * 1.8f, textHeight / 60.0f * 1.8f, 1.0f);
		}
	}

	void ExplosionEffect::init(const glm::vec2 & basePosition, const std::string& specificSound)
	{
		m_InFisrt = true;
		m_OnUsing = true;

		m_CircleIndex = m_StartCircleIndex;
		m_TextIndex = m_StartTextIndex;
		m_EllipseIndex = m_StartEllipseIndex;

		m_pastTime = 0.0f;
		m_TextPastTime = 0.0f;

		m_CircleTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(basePosition, ZOrder::z_ExplosionCircle))* glm::scale(glm::mat4(1.0f), m_CircleScale);
		m_EllipseTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(basePosition, ZOrder::z_ExplosionEllipse))* glm::scale(glm::mat4(1.0f), m_EllipseScale);

		if (m_Text)
		{
			m_TextTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(basePosition, ZOrder::z_ExplosionText))* glm::scale(glm::mat4(1.0f), m_TextScale);
		}

		if (specificSound == "")
		{
			PLAY_SOUND_NAME("Explosion" + std::to_string(Gear::Util::GetRndInt(2) + 1), WormsSound::Weapon);
		}
		else if (specificSound == "None")
		{
			//none sound
		}
		else
		{
			PLAY_SOUND_NAME(specificSound, WormsSound::Weapon);
		}
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
		}

		if (m_Text)
		{
			m_TextPastTime += ts;
			if (m_TextFrameDelay < m_TextPastTime)
			{
				m_TextPastTime = 0.0f;
				--m_TextIndex;
			}
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
		if (m_TextIndex >= 0 && m_Text)
		{
			Gear::Renderer2D::DrawFrameQuad(m_TextTranslate, m_Text, 0, m_TextIndex);
		}
	}

	void ExplosionSmokeBunddle::ExplosionSmoke::init(Explosion::Size size, const glm::vec2& offsetVector, const glm::vec2 & externalVector)
	{
		switch (size)
		{
		case InGame::Explosion::Size25:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("ExplosionSmoke25");
			m_StartIndex = 21;
			break;
		case InGame::Explosion::Size50:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("ExplosionSmoke50");
			m_StartIndex = 27;
			break;
		case InGame::Explosion::Size75:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("ExplosionSmoke75");
			m_StartIndex = 27;
			break;
		case InGame::Explosion::Size100:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("ExplosionSmoke100");
			m_StartIndex = 27;
			break;
		}
		int width = m_Texture->GetWidth();
		int height = m_Texture->GetHeight();
		m_Scale = glm::vec3(width / 60.0f * 1.8f, height / 60.0f * 1.8f, 1.0f);

		m_OriginTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(offsetVector, ZOrder::z_ExplosionSmoke)) * glm::scale(glm::mat4(1.0f), m_Scale);
		m_OriginExternalVector = externalVector;
	}

	void ExplosionSmokeBunddle::ExplosionSmoke::init(const glm::vec2& worldPosition)
	{
		m_ExternalVector.x = m_OriginExternalVector.x;
		m_ExternalVector.y = m_OriginExternalVector.y;
		m_Translate = m_OriginTranslate;
		m_Translate[3][0] += worldPosition.x;
		m_Translate[3][1] += worldPosition.y;

		m_TextureIndex = m_StartIndex;

		m_OnUsing = true;
		m_PastTime = 0.0f;
	}

	void ExplosionSmokeBunddle::ExplosionSmoke::Update(float ts)
	{
		if (!m_OnUsing)
			return;

		m_PastTime += ts;


		m_Translate[3][0] += m_ExternalVector.x * ts;
		m_Translate[3][1] += m_ExternalVector.y * ts;

		if (m_PastTime > m_FrameDelay)
		{
			m_PastTime = 0.0f;
			--m_TextureIndex;
			if (m_TextureIndex < 0)
			{
				m_OnUsing = false;
			}
		}
	}

	void ExplosionSmokeBunddle::ExplosionSmoke::Render()
	{
		if (m_OnUsing)
		{
			Gear::Renderer2D::DrawFrameQuad(m_Translate, m_Texture, 0, m_TextureIndex);
		}
	}

	void ExplosionSmokeBunddle::init(Explosion::Size size)
	{
		glm::vec2 offsetVector(0.0f);
		switch (size)
		{
		case InGame::Explosion::Size25:
			m_Smokes.resize(9);
			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					offsetVector.x = 0.1f * j;
					offsetVector.y = 0.1f * i;
					m_Smokes[(i + 1) * 3 + (j + 1)].init(size, offsetVector, glm::vec2(Gear::Util::GetRndFloatFromTo(-2.2f, 2.2f), Gear::Util::GetRndFloatFromTo(-0.5f, 5.0f)));
				}
			}
			break;
		case InGame::Explosion::Size50:
			m_Smokes.resize(12);
			for (int i = -1; i <= 1; ++i)
			{
				for (float j = -1.5f; j <= 1.5f; j += 1.0f)
				{
					offsetVector.x = 0.15f * j;
					offsetVector.y = 0.15f * i;
					m_Smokes[(i + 1) * 4 + (int)(j + 1.5f)].init(size, offsetVector, glm::vec2(Gear::Util::GetRndFloatFromTo(-2.5f, 2.5f), Gear::Util::GetRndFloatFromTo(-0.5f, 6.0f)));
				}
			}
			break;
		case InGame::Explosion::Size75:
			m_Smokes.resize(16);
			for (float i = -1.5; i <= 1.5; i += 1.0f)
			{
				for (float j = -1.5; j <= 1.5f; j += 1.0f)
				{
					offsetVector.x = 0.2f * j;
					offsetVector.y = 0.2f * i;
					m_Smokes[int(i + 1.5f) * 4 + int(j + 1.5f)].init(size, offsetVector, glm::vec2(Gear::Util::GetRndFloatFromTo(-2.8f, 2.8f), Gear::Util::GetRndFloatFromTo(-0.5f, 6.5f)));
				}
			}
			break;
		case InGame::Explosion::Size100:
			m_Smokes.resize(20);
			for (int i = -2; i <= 2; ++i)
			{
				for (float j = -1.5; j <= 1.5f; j += 1.0f)
				{
					offsetVector.x = 0.25f * j;
					offsetVector.y = 0.25f * i;
					m_Smokes[(i + 2) * 4 + int(j + 1.5f)].init(size, offsetVector, glm::vec2(Gear::Util::GetRndFloatFromTo(-3.0f, 3.0f), Gear::Util::GetRndFloatFromTo(-0.5f, 7.0f)));
				}
			}
			break;
		}
	}

	void ExplosionSmokeBunddle::init(const glm::vec2 & worldPosition)
	{
		m_OnUsing = true;
		for (int i = 0; i < m_Smokes.size(); ++i)
		{
			m_Smokes[i].init(worldPosition);
		}
	}

	void ExplosionSmokeBunddle::Update(float ts)
	{
		int usingCount = 0;
		for (auto& smoke : m_Smokes)
		{
			usingCount += smoke.m_OnUsing;
			smoke.Update(ts);
		}
		if (usingCount == 0)
		{
			m_OnUsing = false;
		}
	}

	void ExplosionSmokeBunddle::Render()
	{
		for (auto& smoke : m_Smokes)
		{
			smoke.Render();
		}
	}

	void FlameBundle::Flame::init(const glm::vec2 & externalVector, Explosion::Size size)
	{
		m_OriginExternalVector = externalVector;

		m_Texture = Gear::TextureStorage::GetFrameTexture2D("Flame");
		int width = m_Texture->GetWidth();
		int height = m_Texture->GetHeight();

		m_Scale = glm::vec3(width / 60.0f * 1.8f, height / 60.0f * 1.8f, 1.0f);
		m_OriginTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, ZOrder::z_Flame)) * glm::scale(glm::mat4(1.0f), m_Scale);

		m_OriginExternalVector = externalVector;
		m_StartIndex = 22;
	}

	void FlameBundle::Flame::init(const glm::vec2 & wolrdPosition)
	{
		m_Translate = m_OriginTranslate;
		m_Translate[3][0] += wolrdPosition.x;
		m_Translate[3][1] += wolrdPosition.y;

		m_TextureIndex = Gear::Util::GetRndIntFromTo(m_StartIndex / 3, m_StartIndex);
		m_ExternalVector = m_OriginExternalVector;

		m_PastTime = 0.0f;
		m_GenPastTime = 0.0f;
		m_GenSmokeDelay = Gear::Util::GetRndFloat(0.5f);
		m_OnUsing = true;
	}


	void FlameBundle::Flame::Update(float ts)
	{
		if (!m_OnUsing)
			return;

		m_PastTime += ts;

		m_ExternalVector.y -= m_Gravity * ts;

		if (m_ExternalVector.y < 0.0f)
		{
			m_GenPastTime += ts;
			m_ExternalVector.x *= 0.985f;

			if (m_GenPastTime > m_GenSmokeDelay)
			{
				m_GenPastTime = 0.0f;

				m_GenSmokeDelay = Gear::Util::GetRndFloat(0.5f);

				Gear::Ref<Smoke> smoke = EffectPool::GetSmoke(Explosion::SmokeSize::Size20);

				smoke->init(glm::vec2(m_Translate[3][0], m_Translate[3][1]), glm::vec2(0.0f, 0.05f));
				InGame::ObjectLayer::s_Smokes.push_back(smoke);
			}

			if (m_ExternalVector.y <= -0.1f)
			{
				m_ExternalVector.y = -0.1f;
			}
		}

		m_Translate[3][0] += m_ExternalVector.x;
		m_Translate[3][1] += m_ExternalVector.y;


		if (-14.0f < m_Translate[3][1] && m_Translate[3][1] < -13.0f)
		{
			PLAY_SOUND_NAME("SIZZLE", WormsSound::flame);
			m_OnUsing = false;
		}

		if (m_PastTime > m_FrameDelay)
		{
			m_PastTime = 0.0f;
			--m_TextureIndex;

			if (m_TextureIndex < 0)
			{
				m_OnUsing = false;
			}
		}
	}

	void FlameBundle::Flame::Render()
	{
		if (m_OnUsing)
		{
			Gear::Renderer2D::DrawFrameQuad(m_Translate, m_Texture, 0, m_TextureIndex);
		}
	}

	void FlameBundle::init(Explosion::Size size)
	{
		switch (size)
		{
		case InGame::Explosion::Size25:
			m_Flames.resize(4);
			break;
		case InGame::Explosion::Size50:
			m_Flames.resize(7);
			break;
		case InGame::Explosion::Size75:
			m_Flames.resize(9);
			break;
		case InGame::Explosion::Size100:
			m_Flames.resize(11);
			break;
		}
		for (int i = 0; i < m_Flames.size(); ++i)
		{
			m_Flames[i].init(glm::vec2(Gear::Util::GetRndFloatFromTo(-0.07f, 0.07f), Gear::Util::GetRndFloatFromTo(0.05f, 0.3f)), size);
		}
	}

	void FlameBundle::init(const glm::vec2 & worldPosition)
	{
		m_OnUsing = true;
		for (int i = 0; i < m_Flames.size(); ++i)
		{
			m_Flames[i].init(worldPosition);
		}
	}

	void FlameBundle::Update(float ts)
	{
		int count = 0;
		for (int i = 0; i < m_Flames.size(); ++i)
		{
			m_Flames[i].Update(ts);
			if (m_Flames[i].m_OnUsing)
			{
				++count;
			}
		}
		if (count == 0)
		{
			m_OnUsing = false;
		}
	}


	void FlameBundle::Render()
	{
		for (int i = 0; i < m_Flames.size(); ++i)
		{
			m_Flames[i].Render();
		}
	}

	void Smoke::init(Explosion::SmokeSize size)
	{
		switch (size)
		{
		case InGame::Explosion::Size20:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("Smoke20");
			break;
		case InGame::Explosion::Size30:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("Smoke30");
			break;
		case InGame::Explosion::Size40:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("Smoke40");
			break;
		}

		int width = m_Texture->GetWidth();
		int height = m_Texture->GetHeight();

		m_Scale = glm::vec3(width / 120.0f * 1.8f, height / 120.0f * 1.8f, 1.0f);
		m_OriginTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, ZOrder::z_Smoke)) * glm::scale(glm::mat4(1.0f), m_Scale);

		m_StartIndex = 27;
	}

	void Smoke::init(const glm::vec2& worldPosition, const glm::vec2& externalVector)
	{
		m_ExternalVector = externalVector;

		m_Translate = m_OriginTranslate;

		m_Translate[3][0] += worldPosition.x;
		m_Translate[3][1] += worldPosition.y;

		m_PastTime = 0.0f;
		m_TextureIndex = m_StartIndex;

		m_OnUsing = true;

	}

	void Smoke::Update(float ts)
	{
		if (!m_OnUsing)
			return;

		m_Translate[3][0] += m_ExternalVector.x;
		m_Translate[3][1] += m_ExternalVector.y;

		m_PastTime += ts;

		if (m_PastTime > m_FrameDelay)
		{
			m_PastTime = 0.0f;
			--m_TextureIndex;
			if (m_TextureIndex < 0)
			{
				m_OnUsing = false;
			}
		}

	}

	void Smoke::Render()
	{
		if (m_OnUsing)
		{
			Gear::Renderer2D::DrawFrameQuad(m_Translate, m_Texture, 0, m_TextureIndex);
		}
	}

	void Exhaust::init(ExhaustType::Type type)
	{
		switch (type)
		{
		case InGame::ExhaustType::_1:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("Exhaust1");
			m_StartIndex = 27;
			break;
		case InGame::ExhaustType::Magic:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("MagicExhaust");
			m_StartIndex = 29;
			break;
		}

		int width = m_Texture->GetWidth();
		int height = m_Texture->GetHeight();

		m_Scale = glm::vec3(1.8f, 1.8f, 1.0f);
		m_OriginTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, ZOrder::z_Exhaust)) * glm::scale(glm::mat4(1.0f), m_Scale);
	}

	void Exhaust::init(const glm::vec3 & wolrdPosition)
	{
		m_TextureIndex = m_StartIndex;

		m_Translate = m_OriginTranslate;
		m_Translate[3][0] += wolrdPosition.x;
		m_Translate[3][1] += wolrdPosition.y;

		m_PastTime = 0.0f;
		m_OnUsing = true;
	}

	void Exhaust::Update(float ts)
	{
		if (!m_OnUsing)
			return;

		m_PastTime += ts;
		if (m_PastTime > m_FrameDelay)
		{
			m_PastTime = 0.0f;
			--m_TextureIndex;
			if (m_TextureIndex < 0)
			{
				m_OnUsing = false;
			}
		}
	}

	void Exhaust::Render()
	{
		if (m_OnUsing)
		{
			Gear::Renderer2D::DrawFrameQuad(m_Translate, m_Texture, 0, m_TextureIndex);
		}
	}

	void Spangle::init(TeamColor::Color teamColor)
	{
		switch (teamColor)
		{
		case InGame::TeamColor::Red:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("spangler");
			break;
		case InGame::TeamColor::Blue:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("spangleb");
			break;
		case InGame::TeamColor::Green:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("spangleg");
			break;
		case InGame::TeamColor::Yellow:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("spangley");
			break;
		case InGame::TeamColor::Purple:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("spanglep");
			break;
		case InGame::TeamColor::Sky:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("spanglec");
			break;
		}
		int width = m_Texture->GetWidth();
		int height = m_Texture->GetHeight();
		m_Scale = glm::vec3(width / 60.0f * 1.8f, height / 60.0f  * 1.8f, 1.0f);
	}

	void Spangle::init(const glm::vec2 & worldPosition)
	{
		m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(worldPosition, ZOrder::z_Cursor - 0.1f)) * glm::scale(glm::mat4(1.0f), m_Scale);;
		m_OnUsing = true;
		m_PastTime = 0.0f;
		m_TextureIndex = 30;
	}

	void Spangle::Update(Gear::Timestep ts)
	{
		m_PastTime += ts;

		if (m_PastTime > m_FrameDelay)
		{
			m_PastTime = 0.0f;
			--m_TextureIndex;
			if (m_TextureIndex < 0)
			{
				m_OnUsing = false;
			}
		}
	}

	void Spangle::Render()
	{
		if (m_OnUsing)
		{
			Gear::Renderer2D::DrawFrameQuad(m_Transform, m_Texture, 0, m_TextureIndex);
		}
	}

	void Marker::init(TeamColor::Color teamColor)
	{
		switch (teamColor)
		{
		case InGame::TeamColor::Red:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("markerr");
			break;
		case InGame::TeamColor::Blue:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("markerb");
			break;
		case InGame::TeamColor::Green:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("markerg");
			break;
		case InGame::TeamColor::Yellow:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("markery");
			break;
		case InGame::TeamColor::Purple:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("markerp");
			break;
		case InGame::TeamColor::Sky:
			m_Texture = Gear::TextureStorage::GetFrameTexture2D("markerc");
			break;
		}
		int width = m_Texture->GetWidth();
		int height = m_Texture->GetHeight();
		m_Scale = glm::vec3(width / 60.0f * 1.8f, height / 60.0f  * 1.8f, 1.0f);
	}

	void Marker::init(const glm::vec2 & worldPosition)
	{
		m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(worldPosition, ZOrder::z_Cursor - 0.1f)) * glm::scale(glm::mat4(1.0f), m_Scale);;
		m_OnUsing = true;
		m_OnReverse = false;
		m_PastTime = 0.0f;
		m_LoopCount = 0;
		m_TextureIndex = 9;
	}

	void Marker::Update(Gear::Timestep ts)
	{
		if (m_LoopCount == 4)
		{
			m_OnUsing = false;
		}

		m_PastTime += ts;
		if (m_PastTime > m_FrameDelay)
		{
			if (m_OnReverse)
			{
				++m_TextureIndex;
				if (m_TextureIndex > 9)
				{
					m_TextureIndex = 9;
					m_OnReverse = false;
					++m_LoopCount;
				}
			}
			else
			{
				--m_TextureIndex;
				if (m_TextureIndex < 0)
				{
					m_TextureIndex = 0;
					m_OnReverse = true;
				}
			}
		}
	}

	void Marker::Render()
	{
		if (m_OnUsing)
		{
			Gear::Renderer2D::DrawFrameQuad(m_Transform, m_Texture, 0, m_TextureIndex);
		}
	}

}