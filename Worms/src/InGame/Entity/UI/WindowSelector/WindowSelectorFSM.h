#pragma once

#include "WindowSelectorEventHandler.h"

namespace InGame {

	class OnDropSelectHandler : public Gear::FSM::InputHandler
	{
		bool awake = false;
		bool inFirst = true;
		Gear::Ref<Gear::Status> status;
		int fromID;
		ItemInfo::Number usedItem;

		virtual void Awake(int entityID) override
		{
			status = Gear::EntitySystem::GetStatus(entityID);
		}

		virtual void OnOut(int entityID) override
		{
			inFirst = true;
		}

		void InFirst(int entityID)
		{
			inFirst = false;
			fromID = std::any_cast<int>(status->GetStat(WindowSelectorStat::FromID));
			usedItem = std::any_cast<ItemInfo::Number>(status->GetStat(WindowSelectorStat::ItemNum));
		}

		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (!awake)
			{
				Awake(entityID);
			}
			if (inFirst)
			{
				InFirst(entityID);
			}



		}

	};

	class OnUtilSelectHandler : public Gear::FSM::InputHandler
	{
		bool awake = false;
		bool inFirst = true;
		int fromID;
		ItemInfo::Number usedItem;
		Gear::Ref<Gear::Status> status;

		Gear::Ref<Gear::Texture2D> virtualMouse;
		glm::mat4 virtualMouseTransform;

		virtual void Awake(int entityID) override
		{
			status = Gear::EntitySystem::GetStatus(entityID);
		}

		virtual void OnOut(int entityID) override
		{
			
		}

		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			if (!awake)
			{
				Awake(entityID);
			}

			return WindowSelectorState::OnUtilSelect;
		}

	};

	class OnNotingSelectHandler : public Gear::FSM::InputHandler
	{
		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override
		{
			return WindowSelectorState::OnNothing;




		}

	};

}