#include "wmpch.h"
#include "BackGroundLayer.h"

namespace InGame {

	BackGroundLayer::BackGroundLayer(const InitiateData& initData)
		: Layer("BackGroundLayer"), m_Terrian(new Terrain(initData))
	{
		//Gradient Settup
		m_Grad = Gear::TextureStorage::GetTexture2D("Grad");

		float gradWidth = m_Grad->GetWidth();
		float gradHeight = m_Grad->GetHeight();

		glm::vec3 gradScale(gradWidth / initData.MapReductionRatio, gradHeight / initData.MapReductionRatio, 1.0f);

		m_GradTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.2f))
			* glm::scale(glm::mat4(1.0f), gradScale);

		//water settup
		auto waterTexture = Gear::TextureStorage::GetFrameTexture2D("Water");
		m_Water = Gear::CreateRef<Gear::Animation2D>(waterTexture, 0.05f, true);

		float waterWidth = waterTexture->GetWidth();
		float waterHeight = waterTexture->GetHeight();

		glm::vec3 waterScale(waterWidth / 20.0f, waterHeight / 20.0f, 1.0f);
		for (int i = 0; i < 3; ++i)
		{
			float spare = 10.0f * i;
			for (int j = 0; j < 20; ++j)
			{
				auto waterTranslate = glm::translate(glm::mat4(1.0f), 
					glm::vec3(-60.0f + j * waterScale.x + spare, -15.6f + i * (waterScale.y * 0.4f), 1.0))
					* glm::scale(glm::mat4(1.0f), waterScale);
				m_WaterTranslates.push_back(waterTranslate);
			}
		}
		m_WaterBottomTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -20.0f, 1.0))
			* glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 8.0f, 1.0f));
		m_WaterBottomColor = { 26 / 255.0f, 26 / 255.0f, 50 / 255.0f, 1.0f };
	}

	void BackGroundLayer::OnAttach()
	{
		m_Water->Start();
	}

	void BackGroundLayer::OnDetach()
	{
	}

	void BackGroundLayer::OnUpdate(Gear::Timestep ts)
	{
		m_Water->Update(ts);
		Gear::Renderer2D::DrawQuad(m_GradTranslate, m_Grad);
		Gear::Renderer2D::DrawQuad(m_WaterBottomTranslate, m_WaterBottomColor);
		for (auto waterTranslate : m_WaterTranslates)
		{
			Gear::Renderer2D::DrawAnimation(waterTranslate, m_Water);
		}
		
	}

	void BackGroundLayer::OnImGuiRender()
	{
	}

	void BackGroundLayer::OnEvent(Gear::Event & e)
	{
	}

	/*void BackGroundLayer::DestroyMask(float x, float y, float radius)
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
	}*/

}
