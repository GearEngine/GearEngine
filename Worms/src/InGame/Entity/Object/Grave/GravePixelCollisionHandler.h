#pragma once

#include "GraveFSMHandler.h"

namespace InGame {

	class GraveGenerallPCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		virtual void Handle(int entityID) override
		{

		}


	};

	class GraveOnUnderWaterPCHandler : public Gear::Physics2D::PixelCollisionHander
	{
		virtual void Handle(int entityID) override
		{

		}
	};
}