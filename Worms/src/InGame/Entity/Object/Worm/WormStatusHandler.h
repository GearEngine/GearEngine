#pragma once

namespace InGame {

	class WormDisplayNameHanlder : public Gear::Status::StatusHandler
	{
		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
		}
	};

	class WormDisplayHpHanlder : public Gear::Status::StatusHandler
	{
		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{

		}
	};

}