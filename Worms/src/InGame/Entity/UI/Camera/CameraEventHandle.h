#pragma once

namespace InGame {

	class CameraExplosionEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			GR_TRACE("No.{0} Entity get explosion event", entityID);
			
		}
	};
}