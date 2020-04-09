#include "grpch.h"
#include "Util.h"

namespace Gear {

	namespace Util {

		float GetAngleFromXY(float x, float y)
		{
			float theta = glm::atan(x / y);
			float angle = glm::degrees(theta);

			if (y > 0.0f)
			{
				angle = 90.0f - angle;
			}
			else if (y < 0.0f)
			{
				angle = 270.0f - angle;
			}
			return angle;
		}
	}

}
