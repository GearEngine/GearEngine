#pragma once

namespace InGame {

	class MouseEventHandler : public Gear::EventHandler
	{
		inline virtual void Handle(std::any data, int entityID) override
		{
			GR_TRACE("No.{0} Entity Mouse event", entityID);

		}
	};

}