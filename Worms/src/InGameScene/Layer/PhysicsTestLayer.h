#pragma once
#include "Gear.h"
#include "box2d/box2d.h"
#include "InGameScene/Entity/Box.h"

class PhysicsTestLayer : public Gear::Layer
{
public:
	PhysicsTestLayer();
	virtual ~PhysicsTestLayer() = default;

	virtual void OnAttach() override;
	virtual void OnDetach() override;

	void OnUpdate(Gear::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(Gear::Event& e) override;

private:
	Gear::OrthographicCameraController m_CameraController;

	glm::vec2 MousePosition;
	glm::vec3 MouseWorldPosition;

	//world
	std::unique_ptr<b2World> m_world;
	std::vector<Box> m_boxes;
	Box* mouseBox;
};