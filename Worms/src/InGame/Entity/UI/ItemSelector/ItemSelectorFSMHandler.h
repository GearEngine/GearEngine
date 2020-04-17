#pragma once
#include "ItemSelectorEventHandler.h"

namespace InGame {

	struct ItemSelectorDrawData
	{
		Gear::Ref<Gear::Texture2D> ItemSelectorTexture;
		std::vector<ItemInfo::ItemDescprition> CurrentTeamItemVector;
		std::vector<std::pair<Gear::Ref<Gear::Texture2D>, std::pair<float, float>>> DrawItemInfoList;
		std::vector<std::string> DrawItemTypeStr;

		glm::mat4 Translate;
	};

	class ItemSelectorOnNotActivate : public Gear::FSM::InputHandler
	{
		int mouseID;
		Gear::Ref<Gear::FSM> mouseFSM;

		virtual void Awake(int entityID) override;
		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};


	class ItemSelectorOnEmergy : public Gear::FSM::InputHandler
	{
		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;

	};

	class ItemSelectorOnSelect : public Gear::FSM::InputHandler
	{
		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;

	};

	class ItemSelectorOnSink : public Gear::FSM::InputHandler
	{
		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;

	};
	
	class ItemSelectorOnUpdate : public Gear::FSM::InputHandler
	{
		int worldID;
		Gear::Ref<Gear::Status> worldStatus;
		Gear::Ref<Gear::Status> status;

		virtual void Awake(int entityID);
		virtual Gear::EnumType Handle(int entityID, const Gear::Command& cmd) override;
	};

}