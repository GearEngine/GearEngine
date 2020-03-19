#include "wmpch.h"
#include "Camera.h"

namespace InGame {

	Camera::Camera(float windowWidth, float windowHeight, bool rotate)
	{
		m_CameraController = new Gear::OrthographicCameraController(windowWidth / windowHeight, rotate);
		Gear::Coord2DManger::Get()->SetCamera(m_CameraController);
		Gear::Coord2DManger::Get()->SetResolution(windowWidth, windowHeight);
		
		//Create Entity
		//m_ID = Gear::EntitySystem::CreateEntity(true);

		//Subscpribe EventChannel
		//Gear::EventSystem::SubscribeChannel(m_ID, EventChannel)

	}

	Camera::~Camera()
	{
		delete m_CameraController;
	}

	void Camera::OnUpdate(Gear::Timestep ts)
	{
		m_CameraController->OnUpdate(ts);
	}

	void Camera::OnEvent(Gear::Event & e)
	{
		m_CameraController->OnEvent(e);
	}

}
