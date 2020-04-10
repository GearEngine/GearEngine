#pragma once

namespace InGame {

	namespace Item {

		enum Name : unsigned int
		{
			Bazooka,
			ShotGun,
			Grenade,
			ItemMax
		};

		enum State : unsigned int
		{
			OnGoing,
			OnExplosion,
			OnUnderWater
		};

		enum Info : unsigned int
		{
			Power, Angle, WindAdjustRatio, ExplosionSize,
			ExplosionText
		};


	}
}