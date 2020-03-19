#include "wmpch.h"
#include "Camera.h"

#include "CameraEventHandle.h"
#include "CameraFSMHandler.h"

namespace InGame {

	Camera::Camera(float windowWidth, float windowHeight, bool rotate)
	{
		m_CameraController = new Gear::OrthographicCameraController(windowWidth / windowHeight, rotate);
		Gear::Coord2DManger::Get()->SetCamera(m_CameraController);
		Gear::Coord2DManger::Get()->SetResolution(windowWidth, windowHeight);
		
		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true);

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Transform,	Gear::ComponentID::Controller,
			Gear::ComponentID::FSM,			Gear::ComponentID::Physics,
			Gear::ComponentID::Timer
		});

		//Set Component specific
		Gear::EntitySystem::SetTransform(m_ID, m_CameraController->GetCamera().GetPosition(), 0.0f, glm::vec2(1.0f, 1.0f));
		
		Gear::EntitySystem::SetController(m_ID, {
			{ CameraCommand::MouseMove, MOUSEMOVE_COMMAND }
		});

		Gear::EntitySystem::SetPhysics(m_ID);

		Gear::EntitySystem::SetFSM(m_ID, {
			{ CameraState::OnFlowing, new CameraOnFlowingHandler }, { CameraState::OnMoveWithMouse, new CameraOnMoveWithMouseHandler },
			{ CameraState::OnStop, new CameraOnStopHandler }
		});

		//Subscpribe EventChannel
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::Explosion, Gear::CreateRef<CameraExplosionEventHandler>());
	}

	Camera::~Camera()
	{
		delete m_CameraController;
		Gear::EventSystem::UnSubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EntitySystem::DeleteEntity(m_ID);
	}

	void Camera::OnUpdate(Gear::Timestep ts)
	{
		auto position = Gear::EntitySystem::GetTransform2D(m_ID)->GetPosition();
		m_CameraController->GetCamera().SetPosition(position);
	}

	void Camera::OnEvent(Gear::Event & e)
	{
	}

}
