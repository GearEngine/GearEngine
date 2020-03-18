#include "TerrainLayer.h"
#include "imgui/imgui.h"

///////////////////////////////////////////
//////// TerrainLayer  ////////////////////
///////////////////////////////////////////

TerrainLayer::TerrainLayer()
{
	m_Mask = Gear::TextureStorage::GetTexture2D("Terrain_Mask");
	m_Terrian = Gear::TextureStorage::GetTexture2D("Terrain");

	m_TerrainScale = { m_Terrian->GetWidth() / 20.0f , m_Terrian->GetHeight() / 20.0f };
	m_TerrainTranslate = glm::translate(glm::mat4(1.0f), m_TerrainPosition)
		* glm::scale(glm::mat4(1.0f), { m_TerrainScale.x, m_TerrainScale.y, 1.0f });
}

void TerrainLayer::OnAttach()
{
}

void TerrainLayer::OnDetach()
{
}

void TerrainLayer::OnUpdate(Gear::Timestep ts)
{
	
	m_Mouse = { Gear::Input::GetMouseX(), 720.0f - Gear::Input::GetMouseY() };
	m_MouseOnWorld = Gear::Coord2DManger::Get()->GetWorldPosition_From_ScreenPosition(m_Mouse);
	m_MouseToLocalTexture = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(m_MouseOnWorld, m_TerrainTranslate);

	if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
	{
		DestroyMask(m_MouseToLocalTexture.x, m_MouseToLocalTexture.y, m_Radius);
	}

	Gear::Renderer2D::DrawTextureWithMask(m_TerrainTranslate, m_Terrian, m_Mask);
	Gear::Renderer2D::EndScene();
}

void TerrainLayer::OnImGuiRender()
{
	ImGui::Text("MousePosition		x : %f \t y : %f", m_Mouse.x, m_Mouse.y);
	ImGui::Text("MouseWorldPosition x : %f \t y : %f", m_MouseOnWorld.x, m_MouseOnWorld.y);
	ImGui::Text("MouseLocalPosition x : %f \t y : %f", m_MouseToLocalTexture.x, m_MouseToLocalTexture.y);
	ImGui::DragFloat("Set Explosion radius", &m_Radius, 1.0f, 0.0f, 100.0f);
}

void TerrainLayer::OnEvent(Gear::Event & e)
{
}

void TerrainLayer::DestroyMask(float x, float y, float radius)
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

///////////////////////////////////////////
//////// TerrainLayer  ////////////////////
///////////////////////////////////////////

MaskLayer::MaskLayer()
	: m_CameraController(Gear::WINDOW_WIDTH / (float)Gear::WINDOW_HEIGHT)
{
	m_Mask = Gear::TextureStorage::GetTexture2D("Terrain_Mask");
	m_MaskScale = { m_Mask->GetWidth() / 20.0f , m_Mask->GetHeight() / 20.0f };
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

	Gear::Renderer2D::BeginScene(m_CameraController.GetCamera());
	Gear::Renderer2D::DrawQuad(m_MaskTranslate, m_Mask);
}

void MaskLayer::OnImGuiRender()
{
}

void MaskLayer::OnEvent(Gear::Event & e)
{
	m_CameraController.OnEvent(e);
}

