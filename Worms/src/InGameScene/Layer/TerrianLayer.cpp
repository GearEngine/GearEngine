#include "TerrianLayer.h"
#include "imgui/imgui.h"

TerrianLayer::TerrianLayer()
{
}

void TerrianLayer::OnAttach()
{
}

void TerrianLayer::OnDetach()
{
}

void TerrianLayer::OnUpdate(Gear::Timestep ts)
{
	//Gear::Renderer2D::DrawQuad({ 0.0f, -3.0f }, { 5.0f, 5.0f }, m_Terrian);
	Gear::Renderer2D::EndScene();
}

void TerrianLayer::OnImGuiRender()
{
}

void TerrianLayer::OnEvent(Gear::Event & e)
{
}

MaskLayer::MaskLayer()
	: m_CameraController(Gear::WINDOW_WIDTH / (float)Gear::WINDOW_HEIGHT)
{
	m_Mask = Gear::TextureStorage::GetTexture2D("Terrian_Mask");
	m_Terrian = Gear::TextureStorage::GetTexture2D("Terrian");

	m_MaskScale = { m_Terrian->GetWidth() / 20.0f , m_Terrian->GetHeight() / 20.0f };

	m_MaskTranslate = glm::translate(glm::mat4(1.0f), m_MaskPosition)
		* glm::scale(glm::mat4(1.0f), { m_MaskScale.x, m_MaskScale.y, 1.0f });

	Gear::Coord2DManger::Get()->SetCamera(&m_CameraController);
	Gear::Coord2DManger::Get()->SetResolution(Gear::WINDOW_WIDTH, Gear::WINDOW_HEIGHT);
}

void MaskLayer::OnAttach()
{
}

void MaskLayer::OnDetach()
{
}

void MaskLayer::OnUpdate(Gear::Timestep ts)
{
	m_CameraController.OnUpdate(ts);

	Gear::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
	Gear::RenderCommand::Clear();

	m_Mouse = { Gear::Input::GetMouseX(), 720.0f - Gear::Input::GetMouseY() };
	m_MouseOnWorld = Gear::Coord2DManger::Get()->GetWorldPosition_From_ScreenPosition(m_Mouse);
	m_MouseToLocalTexture = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(m_MouseOnWorld, m_MaskTranslate);

	if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
	{
		DestroyMask(m_MouseToLocalTexture.x, m_MouseToLocalTexture.y, m_Radius);
	}

	Gear::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Gear::Renderer2D::DrawTextureWithMask(m_MaskTranslate, m_Terrian, m_Mask);
}

void MaskLayer::OnImGuiRender()
{
	ImGui::Text("MousePosition		x : %f \t y : %f", m_Mouse.x, m_Mouse.y);
	ImGui::Text("MouseWorldPosition x : %f \t y : %f", m_MouseOnWorld.x, m_MouseOnWorld.y);
	ImGui::Text("MouseLocalPosition x : %f \t y : %f", m_MouseToLocalTexture.x, m_MouseToLocalTexture.y);
	ImGui::DragFloat("Set Explosion radius", &m_Radius, 1.0f, 0.0f, 100.0f);
}

void MaskLayer::OnEvent(Gear::Event & e)
{
	m_CameraController.OnEvent(e);
}

void MaskLayer::DestroyMask(float x, float y, float radius)
{
	int _width = m_Mask->GetWidth();
	int _height = m_Mask->GetHeight();
	unsigned int* _data = (unsigned int*)m_Mask->GetData();

	float _x = x * _width;
	float _y = y * _height;
	float _radius = radius;

	int mHeight = -(_radius + 1);
	for (int yPos = _y - _radius; yPos <= _y + _radius; ++yPos)
	{
		++mHeight;
		if (yPos < 0 || yPos >= _height)
		{
			continue;
		}
		float platwidth = std::sqrt(std::powf(_radius, 2) - std::powf(mHeight, 2));

		for (int xPos = _x - platwidth; xPos < _x + platwidth; ++xPos)
		{
			if (xPos < 0 || xPos >= _width)
			{
				continue;
			}
			_data[_width * yPos + xPos] = 0;
		} 
	}
	m_Mask->SetData(_data, _width * _height * 4);
}
