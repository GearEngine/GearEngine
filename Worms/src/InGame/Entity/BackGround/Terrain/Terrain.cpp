#include "wmpch.h"
#include "Terrain.h"
#include "TerrainEventHandler.h"

namespace InGame {

	Terrain::Terrain(const InitiateData& initData)
	{
		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true);

		//Attach Component
		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Texturer, Gear::ComponentID::Drawer,
			Gear::ComponentID::Transform
		});

		auto map = Gear::TextureStorage::GetTexture2D(initData.Mapinfo.MapName);
		auto mask = Gear::TextureStorage::GetTexture2D(initData.Mapinfo.MapName + "Mask");
		int width = map->GetWidth();
		int height = map->GetHeight();

		//Set Component specific
		Gear::EntitySystem::SetTransform(m_ID, initData.MapPosition, 0.0f, 
			glm::vec2(width / initData.MapReductionRatio, height / initData.MapReductionRatio));

		Gear::EntitySystem::SetTexturer(m_ID, Gear::RenderType::Masking, map, mask);

		//Subscpribe EventChannel
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventType::Explosion, Gear::CreateRef<TerrainExplosionEventHandler>());
	}

	Terrain::~Terrain()
	{
		Gear::EventSystem::UnSubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EntitySystem::DeleteEntity(m_ID);
	}

}

