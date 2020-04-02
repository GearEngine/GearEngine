#pragma once

namespace InGame {

	class TerrainExplosionEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			GR_TRACE("No.{0} Entity get explosion event", entityID);

			auto explosion = std::any_cast<ExplosionData>(data);

			GR_TRACE("No.{0} Entity : Terrain Damaged by explosion", entityID);
			handled = true;
		}
	};

}