#include "wmpch.h"
#include "Camera.h"

#include "CameraEventHandle.h"
#include "CameraFSMHandler.h"

namespace InGame {

	Camera::Camera(const InitiateData& initData)
	{
		m_CameraController = new Gear::OrthographicCameraController(initData.WindowWidth / initData.WindowHeight);
		Gear::Coord2DManger::Get()->SetCamera(m_CameraController);
		Gear::Coord2DManger::Get()->SetResolution(initData.WindowWidth, initData.WindowHeight);

		//Create Entity
		m_ID = Gear::EntitySystem::CreateEntity(true);

		Gear::EntitySystem::AttachComponent(m_ID, {
			Gear::ComponentID::Transform,	Gear::ComponentID::FSM,			
			Gear::ComponentID::Physics,		Gear::ComponentID::Timer
		});

		//Set Component specific
		Gear::EntitySystem::SetTransform(m_ID, m_CameraController->GetCamera().GetPosition(), 0.0f, glm::vec2(1.0f, 1.0f));

		Gear::EntitySystem::SetPhysics(m_ID);
		Gear::EntitySystem::SetMoveLimit(m_ID, initData.CameraLimit);
		Gear::EntitySystem::SetSliding(m_ID, initData.CameraSliding);

		Gear::EntitySystem::SetFSM(m_ID, {
			{ CameraState::OnFlowing, new CameraOnFlowingHandler }, { CameraState::OnStop, new CameraOnStopHandler }
		});

		//Subscpribe EventChannel
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::MouseMove);
		Gear::EventSystem::SubscribeChannel(m_ID, EventChannel::World);

		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::Explosion, Gear::CreateRef<CameraExplosionEventHandler>());
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::MouseMove, Gear::CreateRef<CameraMouseMoveEventHandler>());
		Gear::EventSystem::RegisterEventHandler(m_ID, EventChannel::World, Gear::CreateRef<>)
	}

	Camera::~Camera()
	{
		delete m_CameraController;
		Gear::EventSystem::UnSubscribeChannel(m_ID, EventChannel::Explosion);
		Gear::EventSystem::UnSubscribeChannel(m_ID, EventChannel::MouseMove);
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
