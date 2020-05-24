#pragma once
#include "WindowSelectorEnum.h"

namespace InGame {

	class WindowSelectorMouseEventHandler : public Gear::EventHandler
	{
		bool awake = false;
		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Status> Status;
		Gear::Ref<Gear::Texture2D> mask;

		int dx[5] = { -3, -3, 3, 3, 0};
		int dy[5] = { 6, -6, 6, -6, 0};

		void Awake(int entityID)
		{
			awake = true;
			FSM = Gear::EntitySystem::GetFSM(entityID);
			Status = Gear::EntitySystem::GetStatus(entityID);
			auto TerrainID = Gear::EntitySystem::GetEntityIDFromName("Terrain");
			mask = Gear::EntitySystem::GetTexturer(TerrainID)->GetMask();
		}

		bool isInTerrain(glm::vec2& worldPosition, int fromID);

		virtual void Handle(std::any data, int entityID, bool& handled) override;

	};

	class WindowSelectorWormEventHandler : public Gear::EventHandler
	{
		int isAwake = true;

		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Status> Status;

		void Awake(int entityID)
		{
			isAwake = false;
			FSM = Gear::EntitySystem::GetFSM(entityID);
			Status = Gear::EntitySystem::GetStatus(entityID);
		}

		virtual void Handle(std::any data, int entityID, bool& handled) override;


	};

}