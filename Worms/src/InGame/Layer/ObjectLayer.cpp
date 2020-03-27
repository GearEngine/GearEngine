#include "wmpch.h"
#include "ObjectLayer.h"

#include "InGame/Entity/Object/Worm/WormEnum.h"

namespace InGame {

	int ObjectLayer::m_nWorms = 0;
	int ObjectLayer::m_CurrentActiveWorm = 0;
	bool ObjectLayer::m_turnChanged = false;

	ObjectLayer::ObjectLayer(const InitiateData& initData)
		: Layer("ObjectLayer")
	{
		m_nWorms = initData.Worms.size();
		m_Worms.resize(m_nWorms);
		for (int i = 0; i < m_Worms.size(); ++i)
		{
			m_Worms[i].reset(new Worm(i, initData));
		}

		m_Transceiver = Gear::EntitySystem::CreateEntity(true);

		Gear::EventSystem::SubscribeChannel(m_Transceiver, EventChannel::World);
		Gear::EventSystem::RegisterEventHandler(m_Transceiver, EventChannel::World, Gear::CreateRef<ObjLayerTransceiver>());
	}

	void ObjectLayer::OnAttach()
	{
		for (auto e : m_Worms)
		{
			Gear::EntitySystem::ActivateComponent(e->GetID(), { Gear::ComponentID::Physics });
		}
	}

	void ObjectLayer::OnDetach()
	{
		m_Worms.clear();
	}

	void ObjectLayer::OnUpdate(Gear::Timestep ts)
	{
		if (m_turnChanged)
		{
			HandleTurnChange();
		}
	}

	void ObjectLayer::OnImGuiRender()
	{
	}

	void ObjectLayer::OnEvent(Gear::Event & e)
	{
	}

	void ObjectLayer::HandleTurnChange()
	{
		m_turnChanged = false;
		for (int i = 0; i < m_Worms.size(); ++i)
		{
			if (i == m_CurrentActiveWorm)
			{
				Gear::EntitySystem::ActivateComponent(m_Worms[i]->GetID(), { {Gear::ComponentID::Controller} });
				auto FSM = Gear::EntitySystem::GetFSM(m_Worms[i]->GetID());
				FSM->SetCurrentState(WormState::OnReady);
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewFollow, m_Worms[i]->GetID())));
			}
			else
			{
				Gear::EntitySystem::InActivateComponent(m_Worms[i]->GetID(), { {Gear::ComponentID::Controller} });
			}
		}
	}

	void ObjectLayer::ChangeWorm()
	{
		m_CurrentActiveWorm++;
		if (m_CurrentActiveWorm >= m_nWorms)
		{
			m_CurrentActiveWorm = 0;
		}
		m_turnChanged = true;
	}

}

