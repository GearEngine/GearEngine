#include "wmpch.h"
#include "Grave.h"

#include "GravePixelCollisionHandler.h"

namespace InGame {

	GravePool* GravePool::s_Inst = nullptr;

	Grave::Grave(const InitiateData& initData, GraveInfo::Type type)
	{
		m_ID = Gear::EntitySystem::CreateEntity();

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Animator, Gear::ComponentID::FSM,
			Gear::ComponentID::Drawer, Gear::ComponentID::Transform,
			Gear::ComponentID::Physics, 
		});

		Gear::EntitySystem::SetFSM(m_ID, {
			{ GraveInfo::State::OnNothing, new GraveOnNothingHandler },
			{ GraveInfo::State::OnUnderWater, new GraveOnUnderWaterHandler }
		});

		auto mask = initData.Mapinfo.Mask;
		int width = mask->GetWidth();
		int height = mask->GetHeight();
		auto maskTranslate = glm::translate(glm::mat4(1.0f), initData.MapPosition)
			* glm::scale(glm::mat4(1.0f), { width / initData.MapReductionRatio  , height / initData.MapReductionRatio , 1.0f });

		std::vector<std::pair<int, int>> graveAniOrder;
		for (int i = 0; i < 40; ++i)
		{
			if (i < 20)
			{
				graveAniOrder.push_back({ 0, 19 - i });
			}
			else
			{
				graveAniOrder.push_back({ 0, i - 20 });
			}
		}

		switch (type)
		{
		case InGame::GraveInfo::_1:
		{
			auto underwater = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave1"), 0.02f, graveAniOrder, true);
			underwater->SetTintColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
			Gear::EntitySystem::SetAnimator(m_ID, {
				{ GraveInfo::State::OnNothing, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave1"), 0.02f,graveAniOrder, true)},
				{ GraveInfo::State::OnUnderWater, underwater}
			});
		}
		break;
		case InGame::GraveInfo::_2:
		{
			auto underwater = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave2"), 0.02f, graveAniOrder, true);
			underwater->SetTintColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
			Gear::EntitySystem::SetAnimator(m_ID, {
				{ GraveInfo::State::OnNothing, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave2"), 0.02f,graveAniOrder, true)},
				{ GraveInfo::State::OnUnderWater, underwater}
			});
		}
		break;
		case InGame::GraveInfo::_3:
		{
			auto underwater = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave3"), 0.02f, graveAniOrder, true);
			underwater->SetTintColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
			Gear::EntitySystem::SetAnimator(m_ID, {
				{ GraveInfo::State::OnNothing, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave3"), 0.02f,graveAniOrder, true)},
				{ GraveInfo::State::OnUnderWater, underwater}
			});
		}
		break;
		case InGame::GraveInfo::_4:
		{
			auto underwater = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave4"), 0.02f, graveAniOrder, true);
			underwater->SetTintColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
			Gear::EntitySystem::SetAnimator(m_ID, {
				{ GraveInfo::State::OnNothing, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave4"), 0.02f,graveAniOrder, true)},
				{ GraveInfo::State::OnUnderWater, underwater}
				});
		}
		break;
		case InGame::GraveInfo::_5:
		{
			auto underwater = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave5"), 0.02f, graveAniOrder, true);
			underwater->SetTintColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
			Gear::EntitySystem::SetAnimator(m_ID, {
				{ GraveInfo::State::OnNothing, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave5"), 0.02f, graveAniOrder, true)},
				{ GraveInfo::State::OnUnderWater, underwater}
			});
		}
		break;
		case InGame::GraveInfo::_6:
		{
			auto underwater = Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave6"), 0.02f, graveAniOrder, true);
			underwater->SetTintColor(glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
			Gear::EntitySystem::SetAnimator(m_ID, {
				{ GraveInfo::State::OnNothing, Gear::Animation2D::Create(Gear::TextureStorage::GetFrameTexture2D("Grave6"), 0.02f, graveAniOrder, true)},
				{ GraveInfo::State::OnUnderWater, underwater}
			});
		}
		break;
		}

		Gear::EntitySystem::SetPixelCollision(m_ID, { 255, 255, 255 }, mask, maskTranslate, {
			{ "General", Gear::CreateRef<GraveGenerallPCHandler>() },
			{ "UnderWater", Gear::CreateRef<GraveOnUnderWaterPCHandler>() },
		});

		Gear::EntitySystem::SetPhysics(m_ID, true);
		auto physics = Gear::EntitySystem::GetPhysics2D(m_ID);
		physics->SetPixelCollisionHandler("General");

		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::Explosion, Gear::CreateRef<GraveExplosionEventHandler>());
	}

	void Grave::init(const glm::vec2 & wolrdPosition)
	{
		Gear::EntitySystem::SetTransform(m_ID, glm::vec3(wolrdPosition, ZOrder::z_Grave), 0.0f, glm::vec3(1.8f, 1.8f, 1.0f));
		Gear::EntitySystem::ActivateEntity(m_ID);
		Gear::EntitySystem::GetAnimator2D(m_ID)->PlayAnimation(GraveInfo::OnNothing);
	}

	GravePool::~GravePool()
	{
		m_GraveType1.clear();
		m_GraveType2.clear();
		m_GraveType3.clear();
		m_GraveType4.clear();
		m_GraveType5.clear();
		m_GraveType6.clear();
	}

	void GravePool::init(const InitiateData & initData)
	{
		m_GraveType1.clear();
		m_GraveType2.clear();
		m_GraveType3.clear();
		m_GraveType4.clear();
		m_GraveType5.clear();
		m_GraveType6.clear();

		m_GraveType1.resize(15);
		for (int i = 0; i < 15; ++i)
		{
			m_GraveType1[i].reset(new Grave(initData, GraveInfo::Type::_1));
		}
		m_GraveType2.resize(15);
		for (int i = 0; i < 15; ++i)
		{
			m_GraveType2[i].reset(new Grave(initData, GraveInfo::Type::_2));
		}
		m_GraveType3.resize(15);
		for (int i = 0; i < 15; ++i)
		{
			m_GraveType3[i].reset(new Grave(initData, GraveInfo::Type::_3));
		}
		m_GraveType4.resize(15);
		for (int i = 0; i < 15; ++i)
		{
			m_GraveType4[i].reset(new Grave(initData, GraveInfo::Type::_4));
		}
		m_GraveType5.resize(15);
		for (int i = 0; i < 15; ++i)
		{
			m_GraveType5[i].reset(new Grave(initData, GraveInfo::Type::_5));
		}
		m_GraveType6.resize(15);
		for (int i = 0; i < 15; ++i)
		{
			m_GraveType6[i].reset(new Grave(initData, GraveInfo::Type::_6));
		}
	}

	void GravePool::ActivateGrave(GraveInfo::Type type, const glm::vec2 & worldPosition)
	{
		switch (type)
		{
		case InGame::GraveInfo::_1:
			while (1)
			{
				++m_GraveType1Ptr;
				if (m_GraveType1Ptr >= 15)
				{
					m_GraveType1Ptr = 0;
				}
				if (!Gear::EntitySystem::IsEntityActivated(m_GraveType1[m_GraveType1Ptr]->m_ID))
				{
					m_GraveType1[m_GraveType1Ptr]->init(worldPosition);
					return;
				}
			}
		case InGame::GraveInfo::_2:
			while (1)
			{
				++m_GraveType2Ptr;
				if (m_GraveType2Ptr >= 15)
				{
					m_GraveType2Ptr = 0;
				}
				if (!Gear::EntitySystem::IsEntityActivated(m_GraveType2[m_GraveType2Ptr]->m_ID))
				{
					m_GraveType2[m_GraveType2Ptr]->init(worldPosition);
					return;
				}
			}
		case InGame::GraveInfo::_3:
			while (1)
			{
				++m_GraveType3Ptr;
				if (m_GraveType3Ptr >= 15)
				{
					m_GraveType3Ptr = 0;
				}
				if (!Gear::EntitySystem::IsEntityActivated(m_GraveType3[m_GraveType3Ptr]->m_ID))
				{
					m_GraveType3[m_GraveType3Ptr]->init(worldPosition);
					return;
				}
			}
		case InGame::GraveInfo::_4:
			while (1)
			{
				++m_GraveType4Ptr;
				if (m_GraveType4Ptr >= 15)
				{
					m_GraveType4Ptr = 0;
				}
				if (!Gear::EntitySystem::IsEntityActivated(m_GraveType4[m_GraveType4Ptr]->m_ID))
				{
					m_GraveType4[m_GraveType4Ptr]->init(worldPosition);
					return;
				}
			}
		case InGame::GraveInfo::_5:
			while (1)
			{
				++m_GraveType5Ptr;
				if (m_GraveType5Ptr >= 15)
				{
					m_GraveType5Ptr = 0;
				}
				if (!Gear::EntitySystem::IsEntityActivated(m_GraveType5[m_GraveType5Ptr]->m_ID))
				{
					m_GraveType5[m_GraveType5Ptr]->init(worldPosition);
					return;
				}
			}
		case InGame::GraveInfo::_6:
			while (1)
			{
				++m_GraveType6Ptr;
				if (m_GraveType6Ptr >= 15)
				{
					m_GraveType6Ptr = 0;
				}
				if (!Gear::EntitySystem::IsEntityActivated(m_GraveType6[m_GraveType6Ptr]->m_ID))
				{
					m_GraveType6[m_GraveType6Ptr]->init(worldPosition);
					return;
				}
			}
		}
	}


	GravePool * GravePool::Get()
	{
		if (s_Inst == nullptr)
		{
			return s_Inst = new GravePool;
		}
		return s_Inst;
	}

	void GravePool::Destroy()
	{
		if (s_Inst != nullptr)
		{
			delete s_Inst;
		}
	}




}