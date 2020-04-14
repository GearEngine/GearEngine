#include "wmpch.h"
#include "Grave.h"

#include "GravePixelCollisionHandler.h"

namespace InGame {

	GravePool* s_Inst = nullptr;


	Grave::Grave(const InitiateData& initData)
	{
		m_ID = Gear::EntitySystem::CreateEntity();

		Gear::EntitySystem::ActivateComponent(m_ID, {
			Gear::ComponentID::Animator, Gear::ComponentID::Drawer,
			Gear::ComponentID::Drawer, Gear::ComponentID::Transform,
			Gear::ComponentID::Physics, Gear::ComponentID::FSM
		});

		Gear::EntitySystem::SetFSM(m_ID, {
			{ GraveInfo::State::OnNothing, new GraveOnNothingHandler },
			{ GraveInfo::State::OnUnderWater, new GraveOnUnderWaterHandler }
		});

		auto mask = Gear::TextureStorage::GetTexture2D(initData.Mapinfo.MapName + "Mask");
		int width = mask->GetWidth();
		int height = mask->GetHeight();
		auto maskTranslate = glm::translate(glm::mat4(1.0f), initData.MapPosition)
			* glm::scale(glm::mat4(1.0f), { width / initData.MapReductionRatio  , height / initData.MapReductionRatio , 1.0f });

		Gear::EntitySystem::SetPixelCollision(m_ID, { 255, 255, 255 }, mask, maskTranslate, {
			{ "General", Gear::CreateRef<GraveGenerallPCHandler>() },
			{ "UnderWater", Gear::CreateRef<GraveOnUnderWaterPCHandler>() },
		});

		Gear::EntitySystem::SetPhysics(m_ID);
		auto physics = Gear::EntitySystem::GetPhysics2D(m_ID);
		physics->SetPixelCollisionHandler("General");
	}

	void GravePool::init(const InitiateData & initData)
	{

		for (int i = 0; i < 15; ++i)
		{

		}



	}

	void GravePool::init(GraveInfo::Type graveType, const glm::vec2 & wolrdPosition)
	{
		switch (graveType)
		{
		case InGame::GraveInfo::_1:
			break;
		case InGame::GraveInfo::_2:
			break;
		case InGame::GraveInfo::_3:
			break;
		case InGame::GraveInfo::_4:
			break;
		case InGame::GraveInfo::_5:
			break;
		case InGame::GraveInfo::_6:
			break;
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