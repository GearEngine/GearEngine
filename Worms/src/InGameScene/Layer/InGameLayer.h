#pragma once
#include "Gear.h"
#include "InGameScene/Entity/Worm.h"

class InGameLayer : public Gear::Layer
{
public:
	InGameLayer();
	virtual ~InGameLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Gear::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Gear::Event& e) override;

private:
	Gear::OrthographicCameraController m_CameraController;
	Worm worm;

	glm::vec2 MousePosition;
	glm::vec3 MouseWorldPosition;

};