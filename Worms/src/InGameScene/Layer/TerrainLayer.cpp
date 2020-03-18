#include "TerrainLayer.h"
#include "imgui/imgui.h"

///////////////////////////////////////////
//////// TerrainLayer  ////////////////////
///////////////////////////////////////////

TerrainLayer::TerrainLayer()
{
	m_Terrian = Gear::TextureStorage::GetTexture2D("Terrain");
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
	m_MouseToLocalTexture = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(m_MouseOnWorld, Mask::s_MaskTranslate);

	if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
	{
		DestroyMask(m_MouseToLocalTexture.x, m_MouseToLocalTexture.y, m_Radius);
	}
	Gear::Renderer2D::DrawTextureWithMask(Mask::s_MaskTranslate, m_Terrian, Mask::s_Mask);
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
	Camera::OnEvent(e);
}

void TerrainLayer::DestroyMask(float x, float y, float radius)
{
	int _width = Mask::s_Mask->GetWidth();
	int _height = Mask::s_Mask->GetHeight();
	unsigned int* _data = (unsigned int*)Mask::s_Mask->GetData();

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
	Mask::s_Mask->SetData(_data, _width * _height * 4);
}

///////////////////////////////////////////
//////// Mask  ////////////////////////////
///////////////////////////////////////////

Gear::Ref<Gear::Texture2D> Mask::s_Mask = nullptr;
glm::mat4 Mask::s_MaskTranslate = glm::mat4(1.0f);

void Mask::SetMask(std::string maskName, const glm::vec3 & position, const glm::vec2 & scale)
{
	s_Mask = Gear::TextureStorage::GetTexture2D(maskName);
	GR_ASSERT(s_Mask, "There is no mask image {0}", maskName);

	glm::vec2 maskScale = { s_Mask->GetWidth() / scale.x , s_Mask->GetHeight() / scale.y };
	s_MaskTranslate = glm::translate(glm::mat4(1.0f), position)
		* glm::scale(glm::mat4(1.0f), { maskScale.x, maskScale.y, 1.0f });
}

///////////////////////////////////////////
//////// Camera  //////////////////////////
///////////////////////////////////////////
Gear::OrthographicCameraController Camera::s_CameraController = Gear::OrthographicCameraController(1.7f);

void Camera::SetCamera(float windowWidth, float windowHeigth, bool rotate)
{
	s_CameraController = Gear::OrthographicCameraController(windowWidth / windowHeigth, rotate);
	Gear::Coord2DManger::Get()->SetCamera(&s_CameraController);
	Gear::Coord2DManger::Get()->SetResolution(windowWidth, windowHeigth);
}

void Camera::OnUpdate(Gear::Timestep ts)
{
	s_CameraController.OnUpdate(ts);
}

void Camera::OnEvent(Gear::Event & e)
{
	s_CameraController.OnEvent(e);
}
