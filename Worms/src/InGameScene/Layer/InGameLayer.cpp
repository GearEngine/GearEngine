#include "InGameLayer.h"

#include "imgui/imgui.h"
InGameLayer::InGameLayer()
	: Layer("InGameLayer"), m_CameraController(1280.0f / 720.0f), worm(glm::vec3(0.0f, 0.0f, 0.1f))
{

	// Initialize PhysicsWorld!
	gravity.Set(0.0f, -10.f);
	world = new b2World(gravity); 

	// Define Object  
	// < Static Body : Static bodies don't collide with other static bodies and are immovable.>
	//1. Define a body with position, damping, etc.
	groundBodyDef.position.Set(0.0f, -10.f);
	//2. Use the world object to create the body.
	groundBody = world->CreateBody(&groundBodyDef);
	//3. Define fixtures with a shape, friction, density, etc.
	groundBox.SetAsBox(50.0f, 1.0f);
	//4. Create fixtures on the body.
	groundBody->CreateFixture(&groundBox, 0.0f);

	//< Dynamic Body : The main difference, besides dimensions, is that we must establish the dynamic body's mass properties.>
	bodyDef.type = b2_dynamicBody; // Default == static, so we should set b2_dynamicBody.
	bodyDef.position.Set(0.0f, 30.0f);
	body = world->CreateBody(&bodyDef);
	// Set Position
	dynamicBox.SetAsBox(1.0f, 1.0f);
	// Define Fixture
	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	// Set Fixture
	body->CreateFixture(&fixtureDef);

	bodyDef2.type = b2_dynamicBody; // Default == static, so we should set b2_dynamicBody.
	bodyDef2.position.Set(0.0f, 0.0f);
	body2 = world->CreateBody(&bodyDef2);
	// Set Position
	dynamicBox2.SetAsBox(1.0f, 1.0f);
	// Define Fixture
	fixtureDef2.shape = &dynamicBox2;
	fixtureDef2.density = 1.0f;
	fixtureDef2.friction = 0.3f;
	// Set Fixture
	body2->CreateFixture(&fixtureDef2);
}

void InGameLayer::OnAttach()
{
	//AddSound("assets/Sound/show only me.mp3", "bol4", true, true);
	//PlaySound(GetSound("bol4"), SoundChannel::BGM);
}

void InGameLayer::OnDetach()
{
}

void InGameLayer::OnUpdate(Gear::Timestep ts)
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

	Gear::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Gear::Renderer2D::DrawQuad(MouseWorldPosition, { 1.0, 1.0f }, {0.9f, 0.3f, 0.2f, 1.0f});
	Gear::Renderer2D::DrawQuad(m_TestBox , { 1.0, 1.0f }, { 0.9f, 0.3f, 0.2f, 1.0f });
	Gear::Renderer2D::EndScene();

	//Test Simulation
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	
	world->Step(ts, velocityIterations, positionIterations);
	m_TestBox.x = body->GetPosition().x;
	m_TestBox.y = body->GetPosition().y;
	
	body2->SetTransform({ MouseWorldPosition.x, MouseWorldPosition.y }, 0.f);

	float angle = body->GetAngle();
	printf("%4.2f %4.2f %4.2f\n", m_TestBox.x, m_TestBox.y, angle);


	if (Gear::Input::IsKeyPressd(GR_KEY_R))
	{
		
	}
}

void InGameLayer::OnImGuiRender()
{
	ImGui::Text("OnScreen x : %f y : %f", MousePosition.x, MousePosition.y);
	ImGui::Text("OnWorld x : %f y : %f", MouseWorldPosition.x, MouseWorldPosition.y);
}

void InGameLayer::OnEvent(Gear::Event & e)
{
	m_CameraController.OnEvent(e);
}
