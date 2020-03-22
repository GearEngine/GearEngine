#include "wmpch.h"
#include "BackGroundLayer.h"

namespace InGame {

	float BackGroundLayer::s_CurrentWind = 0.0f;

	BackGroundLayer::BackGroundLayer(const InitiateData& initData)
		: Layer("BackGroundLayer"), m_Terrian(new Terrain(initData))
	{
		s_CurrentWind = Gear::GetRndFloatFromTo(-initData.WindMax, initData.WindMax);

		GradSettup(initData);
		WaterSettup(initData);
		FallenLeafSettup(initData);
		CloudSettup(initData);
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
		//Update
		m_Water->Update(ts);

		//Leaf
		for (auto& leafTexture : m_LeafTextures)
		{
			leafTexture->Update(ts);
		}
		for (auto& leafs : m_LeafTranslates)
		{
			for (auto& leaf : leafs)
			{
				leaf.Update(ts);
			}
		}

		//Cloud
		for (auto& cloudTexture : m_CloudTextures)
		{
			cloudTexture->Update(ts);
		}
		for (auto& clouds : m_CloudTranslates)
		{
			for (auto& cloud : clouds)
			{
				cloud.Update(ts);
			}
		}

		//Render
		Gear::Renderer2D::DrawQuad(m_GradTranslate, m_Grad);
		Gear::Renderer2D::DrawQuad(m_WaterBottomTranslate, m_WaterBottomColor);

		//Water
		for (int i = 0 ; i < 75; ++i)
		{
			Gear::Renderer2D::DrawAnimation(m_WaterTranslates[i], m_Water);
		}

		//Leaf
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < m_LeafTranslates[i].size(); ++j)
			{
				Gear::Renderer2D::DrawAnimation(m_LeafTranslates[i][j].Translate, m_LeafTextures[i]);
			}
		}

		//Cloud
		for (int i = 0; i < 3; ++i)
		{
			for (int j = 0; j < m_CloudTranslates[i].size(); ++j)
			{
				Gear::Renderer2D::DrawAnimation(m_CloudTranslates[i][j].Translate, m_CloudTextures[i]);
			}
		}

	}

	void BackGroundLayer::OnImGuiRender()
	{
	}

	void BackGroundLayer::OnEvent(Gear::Event & e)
	{
	}

	void BackGroundLayer::GradSettup(const InitiateData& initData)
	{
		//Gradient Settup
		m_Grad = Gear::TextureStorage::GetTexture2D("Grad");

		float gradWidth = m_Grad->GetWidth();
		float gradHeight = m_Grad->GetHeight();

		glm::vec3 gradScale(gradWidth / GradReductionRatio, gradHeight / GradReductionRatio, 1.0f);

		m_GradTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -0.8f))
			* glm::scale(glm::mat4(1.0f), gradScale);
	}

	void BackGroundLayer::WaterSettup(const InitiateData& initData)
	{
		//water settup
		auto waterTexture = Gear::TextureStorage::GetFrameTexture2D("Water");
		m_Water = Gear::CreateRef<Gear::Animation2D>(waterTexture, 0.05f, true);

		float waterWidth = waterTexture->GetWidth();
		float waterHeight = waterTexture->GetHeight();

		glm::vec3 waterScale(waterWidth / initData.MapReductionRatio, waterHeight / initData.MapReductionRatio, 1.0f);

		float spare[5] = { 3.0f, 7.0f, 4.0f, 2.0f, 5.0f };
		float zOrder;
		for (int i = 0; i < 5; ++i)
		{
			if (i < 3)
			{
				zOrder = 0.1f;
			}
			else
			{
				zOrder = -0.5f;
			}
			for (int j = 0; j < 15; ++j)
			{
				auto waterTranslate = glm::translate(glm::mat4(1.0f),
					glm::vec3(-50.0f + j * waterScale.x + spare[i], -13.3f + i * (waterScale.y * 0.5f), zOrder))
					* glm::scale(glm::mat4(1.0f), waterScale);
				m_WaterTranslates.push_back(waterTranslate);
			}
		}
		m_WaterBottomTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -15.0f, -0.7f))
			* glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 7.5f, 1.0f));
		m_WaterBottomColor = { 50 / 255.0f, 59 / 255.0f, 126 / 255.0f, 1.0f };
	}

	void BackGroundLayer::FallenLeafSettup(const InitiateData& initData)
	{
		m_LeafTextures.resize(10);
		m_LeafTranslates.resize(10);
		auto leaf = Gear::TextureStorage::GetFrameTexture2D("FallenLeaf");
		for (int i = 0; i < 10; ++i)
		{
			m_LeafTextures[i] = Gear::CreateRef<Gear::Animation2D>(leaf, 0.05f, true);
			m_LeafTextures[i]->SetFrameY(i * 10);
			m_LeafTextures[i]->Resume();
			for (int j = 0; j < 10; ++j)
			{
				m_LeafTranslates[i].push_back(Leaf( glm::vec3( Gear::GetRndFloatFromTo(-50.0f, 50.f), Gear::GetRndFloatFromTo(-11.0f, 15.0f), -0.6f ), glm::vec3( 0.6f, 0.6f, 1.0f ), Gear::GetRndFloatFromTo(0.3f, 0.9f)));
				m_LeafTranslates[i][j].RenewWind(s_CurrentWind);
			}
		}
	}


	void BackGroundLayer::CloudSettup(const InitiateData& initData)
	{
		m_CloudTextures.resize(3);
		m_CloudTranslates.resize(3);

		std::vector<std::pair<int, int>> frameOrder;
		for (int i = 0; i < 40; ++i)
		{
			if (i < 20)
			{
				frameOrder.push_back({ 0, i });
			}
			else
			{
				frameOrder.push_back({ 0, 39 - i });
			}
		}
		auto cloudSmall = Gear::TextureStorage::GetFrameTexture2D("CloudSmall");
		auto cloudMiddle = Gear::TextureStorage::GetFrameTexture2D("CloudMiddle");
		auto cloudLarge = Gear::TextureStorage::GetFrameTexture2D("CloudLarge");

		float width[3] = { cloudSmall->GetWidth(), cloudMiddle->GetWidth(), cloudLarge->GetWidth() };
		float height[3] = { cloudSmall->GetHeight(), cloudMiddle->GetHeight(), cloudLarge->GetHeight() };

		m_CloudTextures[0] = Gear::CreateRef<Gear::Animation2D>(cloudSmall, 0.05f, frameOrder, true);
		m_CloudTextures[1] = Gear::CreateRef<Gear::Animation2D>(cloudMiddle, 0.05f, frameOrder, true);
		m_CloudTextures[2] = Gear::CreateRef<Gear::Animation2D>(cloudLarge, 0.05f, frameOrder, true);

		for (int i = 0; i < 3; ++i)
		{
			m_CloudTranslates[i].resize(6);
			m_CloudTextures[i]->Start();
			for (int j = 0; j < 6; ++j)
			{
				float yPos = 14.0f + i * 0.7 + Gear::GetRndFloat(0.3f);
				m_CloudTranslates[i][j] = Cloud(glm::vec3(Gear::GetRndFloatFromTo(-50.0f, 50.0f), yPos, -0.5f - i * 0.01f), glm::vec3(width[i] / initData.MapReductionRatio, height[i] / initData.MapReductionRatio, 1.0f), Gear::GetRndFloatFromTo(-15.0f, 15.0f));
				m_CloudTranslates[i][j].RenewWind(s_CurrentWind);
			}
		}
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

	void Leaf::Update(Gear::Timestep ts)
	{
		Position.x += (Wind * WindResistance) * ts;
		if(WindResistance < 0.4f)
			Position.y -= (Gravity * WindResistance) * ts;
		else
			Position.y -= (Gravity * 0.4f) * ts;

		if (Position.x < -50.0f)
		{
			Position.x = 50.0f;
		}
		else if (Position.x > 50.0f)
		{
			Position.x = -50.0f;
		}

		if (Position.y < -11.0f)
		{
			Position.y = 15.0f;
		}

		Translate = glm::translate(glm::mat4(1.0f), Position)
			* glm::scale(glm::mat4(1.0f), Scale);
	}

	void Cloud::Update(Gear::Timestep ts)
	{
		Position.x += (Wind + BasicMove) * ts * 0.1f;

		if (Position.x > 50.0f)
		{
			Position.x = -50.f;
		}
		else if (Position.x < -50.0f)
		{
			Position.x = 50.0f;
		}
		Translate = glm::translate(glm::mat4(1.0f), Position)
			* glm::scale(glm::mat4(1.0f), Scale);
	}

}
