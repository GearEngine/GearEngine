#pragma once

namespace InGame {

	namespace CameraState {

		enum : unsigned int
		{
			OnFlowing,
			OnMoveWithMouse,
			OnStop,
		};

	}

	namespace CameraCommand {

		enum : unsigned int
		{
			MouseMove
		};
	}

}