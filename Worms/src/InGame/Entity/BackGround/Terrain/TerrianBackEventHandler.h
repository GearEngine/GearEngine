#pragma once

namespace InGame {

	class TerrainBackEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			auto mouseMoveData = std::any_cast<MouseMoveData>(data);
			auto& position = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();
			position.x += mouseMoveData.dx * 0.002f;
			handled = true;
		}
	};

}