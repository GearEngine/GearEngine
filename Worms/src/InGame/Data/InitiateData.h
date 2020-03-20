#pragma once

namespace InGame {

	struct InitiateData
	{
		InitiateData() = default;

		float WindowWidth = Gear::WINDOW_WIDTH;
		float WindowHeight = Gear::WINDOW_HEIGHT;
		int nWorm = 3;

		void* window = Gear::Application::Get().GetWindow().GetNativeWindow();

		std::string MapName = "Terrain";
		glm::vec3 MapPosition = {0.0f, -3.0f, -0.1f};
		float MapReductionRatio = 20.0f;
		Gear::FRect CameraLimit = { -10.0f, 20.0f, 10.0f, 0.0f };
	};

}
