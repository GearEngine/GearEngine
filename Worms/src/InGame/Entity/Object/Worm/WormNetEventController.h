#pragma once

namespace InGame {

	class WormNetEventController : public Gear::NetController::EventController
	{
		virtual void Handle(int entityID, InputMemoryStream& stream);
		void ItemSelectHandle(const WormsPacket::Event& e);
		void TurnChange(const WormsPacket::Event& e);
	};

}