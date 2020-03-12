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
	glm::vec3 m_TestBox = {0.f,0.f,0.f};

	//world
	glm::vec2 MousePosition;
	glm::vec3 MouseWorldPosition;

	//static body
	b2Vec2 gravity;
	b2World* world;
	b2BodyDef groundBodyDef;
	b2Body* groundBody;
	b2PolygonShape groundBox;

	//dynamic body
	b2BodyDef bodyDef;
	b2Body* body;
	b2PolygonShape dynamicBox;
	b2FixtureDef fixtureDef;

	//dynamic body2
	b2BodyDef bodyDef2;
	b2Body* body2;
	b2PolygonShape dynamicBox2;
	b2FixtureDef fixtureDef2;
};