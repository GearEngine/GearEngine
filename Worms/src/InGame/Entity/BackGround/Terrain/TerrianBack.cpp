#include "wmpch.h"
#include "TerrianBack.h"
#include "TerrianBackEventHandler.h"

namespace InGame {

	TerrianBack::TerrianBack(const InitiateData & initData)
	{
		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true);

		//Attach Component
		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Texturer, Gear::ComponentID::Drawer,
			Gear::ComponentID::Transform,
		});

		//Set Component specific
		auto terrainBack = Gear::TextureStorage::GetTexture2D(initData.Mapinfo.TerrianBackName + "Back");
		Gear::EntitySystem::SetTexturer(m_ID, Gear::RenderType::Normal, terrainBack);

		int width = terrainBack->GetWidth();
		int height = terrainBack->GetHeight();
		auto scale = glm::vec2(width / initData.MapReductionRatio, height / initData.MapReductionRatio);
		
		Gear::EntitySystem::SetTransform(m_ID, glm::vec3(0.0f, -9.0f, ZOrder::z_TerrainBack), 0.0f, scale);

		//Subscpribe EventChannel
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::MouseMove);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::MouseMove, Gear::CreateRef<TerrainBackEventHandler>());
	}

}