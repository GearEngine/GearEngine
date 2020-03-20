#pragma once

namespace InGame {

	struct InitiateData
	{
		InitiateData() = default;

		float WindowWidth = Gear::WINDOW_WIDTH;
		float WindowHeight = Gear::WINDOW_HEIGHT;
		int nWorm = 3;

		std::string MapName = "City";
		glm::vec3 MapPosition = {0.0f, -3.0f, -0.1f};
		float MapReductionRatio = 37.0f;

		Gear::FRect CameraLimit = { -30.0f, 30.0f, 30.0f, -7.0f };
	};

}
