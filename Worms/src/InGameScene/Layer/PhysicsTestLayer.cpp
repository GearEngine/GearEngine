#include "PhysicsTestLayer.h"
#include "imgui/imgui.h"
#include <random>

PhysicsTestLayer::PhysicsTestLayer()
	: Layer("PhysicsTestLayer"), m_CameraController(1280.0f / 720.0f)
{
	//m_CameraController.SetZoomLevel(16.0f);  --> 이거 제대로 동작안함.

	b2Vec2 gravity(0.0f, -9.81f);
	m_world = std::make_unique<b2World>(gravity);

	// Make the Ground Definition
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -30.f);

	// Real Ground Body
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);
	//Make the ground fixture
	b2PolygonShape groundBox;
	groundBox.SetAsBox(50.f, 10.f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	//Make a buch of boxes
	std::mt19937 randGenerator;
	std::uniform_real_distribution<float> xPos(-10.0f, 10.0f);
	std::uniform_real_distribution<float> yPos(-15.0f, 15.0f);
	std::uniform_real_distribution<float> size(0.5f, 2.5f);

	const int numBoxes = 150;
	for (int i = 0; i < numBoxes; i++) {
		Box newBox;
		newBox.init(m_world.get(), glm::vec2(xPos(randGenerator), yPos(randGenerator)), glm::vec2(size(randGenerator), size(randGenerator)));
		m_boxes.push_back(newBox);
	}

	mouseBox = new Box();
	mouseBox->init(m_world.get(), { 0.f,0.f }, { 1.5f, 1.5f });
}

void PhysicsTestLayer::OnAttach()
{
}

void PhysicsTestLayer::OnDetach()
{
}

void PhysicsTestLayer::OnUpdate(Gear::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Gear::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Gear::RenderCommand::Clear();

	MousePosition = { Gear::Input::GetMouseX(), 720.0f - Gear::Input::GetMouseY() };
	MouseWorldPosition = glm::inverse(m_CameraController.GetCamera().GetViewProjectionMatrix()) * glm::vec4(MousePosition, 0.0f, 1.0f);

	MouseWorldPosition.x /= 640;
	MouseWorldPosition.y /= 360;

	auto CameraPos = m_CameraController.GetCamera().GetPosition();
	MouseWorldPosition.x += CameraPos.x - 1.77777f * m_CameraController.GetZoomLevel();
	MouseWorldPosition.y += CameraPos.y - 1.0f * m_CameraController.GetZoomLevel();
	mouseBox->getBody()->SetTransform(b2Vec2(MouseWorldPosition.x, MouseWorldPosition.y), 0.f);

	Gear::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Gear::Renderer2D::DrawQuad(MouseWorldPosition, { 1.0, 1.0f }, { 0.9f, 0.3f, 0.2f, 1.0f });

	for (auto& b : m_boxes)
	{
		glm::vec3 destRect;
		destRect.x = b.getBody()->GetPosition().x;
		destRect.y = b.getBody()->GetPosition().y;
		auto width = b.getDimensions().x;
		auto heigh = b.getDimensions().y;
		mouseBox->getBody()->ApplyForce({ 0.0f,1.0f }, { destRect.x, destRect.y }, true);
		Gear::Renderer2D::DrawRotatedQuad({ destRect.x,destRect.y }, { width,heigh }, b.getBody()->GetAngle(), { sinf(destRect.x) , cosf(destRect.y), 0.8f, 1.f });
	}

	Gear::Renderer2D::EndScene();

	m_world->Step(ts, 6, 2);

	if (Gear::Input::IsKeyPressd(GR_KEY_R))
	{

	}
}

void PhysicsTestLayer::OnImGuiRender()
{
	ImGui::Text("OnScreen x : %f y : %f", 0, 0);
	ImGui::Text("OnWorld x : %f y : %f", 0, 0);
}

void PhysicsTestLayer::OnEvent(Gear::Event & e)
{
	m_CameraController.OnEvent(e);
}
