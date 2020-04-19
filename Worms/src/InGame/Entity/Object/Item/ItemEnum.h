#pragma once

namespace InGame {

	namespace Item {

		enum State : unsigned int
		{
			OnGoing,
			OnExplosion,
			OnUnderWater
		};

		enum Info : unsigned int
		{
			Power, Angle, WindAdjustRatio, ExplosionSize, 
			ExplosionText, From, Number, Baby
		};

	}
}