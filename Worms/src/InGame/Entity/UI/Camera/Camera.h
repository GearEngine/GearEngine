#pragma once
#include "Gear.h"

namespace InGame {

	class Camera
	{
	public:
		Camera(float windowWidth, float windowHeight, bool rotate = false);
		~Camera();

		Gear::OrthographicCameraController* m_CameraController;

	public:
		void OnUpdate(Gear::Timestep ts);
		void OnEvent(Gear::Event& e);
	};

}
