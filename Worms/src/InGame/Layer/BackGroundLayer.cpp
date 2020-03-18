#include "BackGroundLayer.h"
#include "imgui/imgui.h"

namespace InGame {

	BackGroundLayer::BackGroundLayer(const InitiateData& initData)
		: Layer("BackGroundLayer")
	{
		m_Terrian = Gear::TextureStorage::GetTexture2D("Terrain");
	}

	void BackGroundLayer::OnAttach()
	{
	}

	void BackGroundLayer::OnDetach()
	{
	}

	void BackGroundLayer::OnUpdate(Gear::Timestep ts)
	{
		m_Mouse = { Gear::Input::GetMouseX(), 720.0f - Gear::Input::GetMouseY() };
		m_MouseOnWorld = Gear::Coord2DManger::Get()->GetWorldPosition_From_ScreenPosition(m_Mouse);
		m_MouseToLocalTexture = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(m_MouseOnWorld, Mask::s_MaskTranslate);

		if (Gear::Input::IsMouseButtonPressed(GR_MOUSE_BUTTON_LEFT))
		{
			DestroyMask(m_MouseToLocalTexture.x, m_MouseToLocalTexture.y, m_Radius);
		}
		Gear::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Gear::RenderCommand::Clear();

		Gear::Renderer2D::BeginScene(Camera::s_CameraController->GetCamera());
		Gear::Renderer2D::DrawTextureWithMask(Mask::s_MaskTranslate, m_Terrian, Mask::s_Mask);
	}

	void BackGroundLayer::OnImGuiRender()
	{
		ImGui::Text("MousePosition		x : %f \t y : %f", m_Mouse.x, m_Mouse.y);
		ImGui::Text("MouseWorldPosition x : %f \t y : %f", m_MouseOnWorld.x, m_MouseOnWorld.y);
		ImGui::Text("MouseLocalPosition x : %f \t y : %f", m_MouseToLocalTexture.x, m_MouseToLocalTexture.y);
		ImGui::DragFloat("Set Explosion radius", &m_Radius, 1.0f, 0.0f, 100.0f);
	}

	void BackGroundLayer::OnEvent(Gear::Event & e)
	{
	}

	void BackGroundLayer::DestroyMask(float x, float y, float radius)
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

}
