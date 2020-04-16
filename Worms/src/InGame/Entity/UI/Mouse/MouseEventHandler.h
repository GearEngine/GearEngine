#pragma once

namespace InGame {

	class MouseEventHandler : public Gear::EventHandler
	{

		inline virtual void Handle(std::any data, int entityID, bool& handled) override
		{
			handled = true;
		}
	};

}