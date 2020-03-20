#pragma once

#include "InGame/Data/InitiateData.h"

namespace InGame {

	class Camera
	{
	public:
		Camera(const InitiateData& initData);
		~Camera();

		Gear::OrthographicCameraController* m_CameraController;

	public:
		void OnUpdate(Gear::Timestep ts);
		void OnEvent(Gear::Event& e);

	private:
		int m_ID;

	};

}
