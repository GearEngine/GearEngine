#include "wmpch.h"
#include "BackGroundLayer.h"

namespace InGame {

	float BackGroundLayer::s_CurrentWind = 0.0f;

	BackGroundLayer::BackGroundLayer(const InitiateData& initData)
		: Layer("BackGroundLayer"), m_Terrian(new Terrain(initData)), m_TerrianBack(new TerrianBack(initData))
	{
		s_CurrentWind = Gear::GetRndFloatFromTo(-initData.WindMax, initData.WindMax);

		GradSettup(initData);
		WaterSettup(initData);
		FloatingSettup(initData);
		CloudSettup(initData);
	}

	void BackGroundLayer::OnAttach()
	{
		m_WaterWave->Start();
	}

	void BackGroundLayer::OnDetach()
	{
	}

	void BackGroundLayer::OnUpdate(Gear::Timestep ts)
	{
		//Update

		//Water
		m_WaterWave->Update(ts);

		//Floating
		for (auto& floatingTexture : m_FloatingTextures)
		{
			floatingTexture->Update(ts);
		}
		for (auto& floatings : m_FloatingTranslates)
		{
			for (auto& floating : floatings)
			{
				floating.Update(ts);
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

		//Grad
		Gear::Renderer2D::DrawQuad(m_GradTranslate, m_Grad);

		//WaterBackGround
		Gear::Renderer2D::DrawQuad(m_WaterBackGroundTranslate, m_WaterBackGroundmColor);

		//WaterWave
		for (int i = 0 ; i < 105; ++i)
		{
			Gear::Renderer2D::DrawAnimation(m_WaterWaveTranslates[i], m_WaterWave);
		}

		//Floating
		for (int i = 0; i < 10; ++i)
		{
			for (int j = 0; j < m_FloatingTranslates[i].size(); ++j)
			{
				Gear::Renderer2D::DrawAnimation(m_FloatingTranslates[i][j].Translate, m_FloatingTextures[i]);
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
		m_Grad = Gear::TextureStorage::GetTexture2D(initData.Mapinfo.Grad);

		float gradWidth = m_Grad->GetWidth();
		float gradHeight = m_Grad->GetHeight();

		glm::vec3 gradScale(gradWidth / 30.0f * 2, gradHeight / 30.0f, 1.0f);

		m_GradTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.0f, ZOrder::z_Grad))
			* glm::scale(glm::mat4(1.0f), gradScale);
	}

	void BackGroundLayer::WaterSettup(const InitiateData& initData)
	{
		//water settup
		auto waterTexture = Gear::TextureStorage::GetFrameTexture2D(initData.Mapinfo.Water);
		m_WaterWave = Gear::CreateRef<Gear::Animation2D>(waterTexture, 0.05f, true);

		float waterWidth = waterTexture->GetWidth();
		float waterHeight = waterTexture->GetHeight();

		glm::vec3 waterScale(waterWidth / initData.MapReductionRatio, waterHeight / initData.MapReductionRatio, 1.0f);

		float WaterOffset[5] = { 3.0f, 1.0f, 0.0f, 2.0f, -1.0f };
		float waterZOrder;
		for (int i = 0; i < 5; ++i)
		{
			if (i < 3)
			{
				waterZOrder = ZOrder::z_WaterWaveF;
			}
			else
			{
				waterZOrder = ZOrder::z_WaterWaveB;
			}
			for (int j = 0; j < 21; ++j)
			{
				auto waterTranslate = glm::translate(glm::mat4(1.0f),
					glm::vec3(initData.WorldRect.Left + j * waterScale.x + WaterOffset[i], -13.3f + i * (waterScale.y * 0.5f), waterZOrder))
					* glm::scale(glm::mat4(1.0f), waterScale);
				m_WaterWaveTranslates.push_back(waterTranslate);
			}
		}
		m_WaterBackGroundTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -15.0f, ZOrder::z_WatterBackGround))
			* glm::scale(glm::mat4(1.0f), glm::vec3(140.0f, 7.5f, 1.0f));
		m_WaterBackGroundmColor = initData.Mapinfo.WaterRGB;
	}

	void BackGroundLayer::FloatingSettup(const InitiateData& initData)
	{
		m_FloatingTextures.resize(10);
		m_FloatingTranslates.resize(10);
		auto leaf = Gear::TextureStorage::GetFrameTexture2D("FallenLeaf");
		for (int i = 0; i < 10; ++i)
		{
			m_FloatingTextures[i] = Gear::CreateRef<Gear::Animation2D>(leaf, 0.05f, true);
			m_FloatingTextures[i]->SetFrameY(i * 10);
			m_FloatingTextures[i]->Resume();
			for (int j = 0; j < 10; ++j)
			{
				m_FloatingTranslates[i].push_back(FloatingMatter(glm::vec3(Gear::GetRndFloatFromTo(initData.WorldRect.Left, initData.WorldRect.Right), Gear::GetRndFloatFromTo(-11.0f, 9.0f), ZOrder::z_FloatingMatter),
					glm::vec3( 0.6f, 0.6f, 1.0f ), Gear::GetRndFloatFromTo(0.3f, 0.9f), initData.WorldRect));
				m_FloatingTranslates[i][j].RenewWind(s_CurrentWind);
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

		float zOrder;
		for (int i = 0; i < 3; ++i)
		{
			m_CloudTranslates[i].resize(8);
			m_CloudTextures[i]->Start();
			if (i == 0)
			{
				zOrder = ZOrder::z_CloudSmall;
			}
			if (i == 1)
			{
				zOrder = ZOrder::z_CloudMiddle;
			}
			if (i == 2)
			{
				zOrder = ZOrder::z_CloudLarge;
			}
			for (int j = 0; j < 8; ++j)
			{
				float yPos = 9.0f + i * 0.7f + Gear::GetRndFloat(0.5f);
				m_CloudTranslates[i][j] = Cloud(glm::vec3(Gear::GetRndFloatFromTo(initData.WorldRect.Left, initData.WorldRect.Right), yPos, zOrder),
					glm::vec3(width[i] / initData.MapReductionRatio, height[i] / initData.MapReductionRatio, 1.0f), Gear::GetRndFloatFromTo(-15.0f, 15.0f), initData.WorldRect);
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

	void FloatingMatter::Update(Gear::Timestep ts)
	{
		Position.x += (Wind * WindResistance) * ts;
		if(WindResistance < 0.4f)
			Position.y -= (Gravity * WindResistance) * ts;
		else
			Position.y -= (Gravity * 0.4f) * ts;

		if (Position.x < WorldRect.Left)
		{
			Position.x = WorldRect.Right;
		}
		else if (Position.x > WorldRect.Right)
		{
			Position.x = WorldRect.Left;
		}

		if (Position.y < -11.0f)
		{
			Position.y = 9.0f;
		}

		Translate = glm::translate(glm::mat4(1.0f), Position)
			* glm::scale(glm::mat4(1.0f), Scale);
	}

	void Cloud::Update(Gear::Timestep ts)
	{
		Position.x += (Wind + BasicMove) * ts * 0.1f;

		if (Position.x > WorldRect.Right)
		{
			Position.x = WorldRect.Left;
		}
		else if (Position.x < WorldRect.Left)
		{
			Position.x = WorldRect.Right;
		}
		Translate = glm::translate(glm::mat4(1.0f), Position)
			* glm::scale(glm::mat4(1.0f), Scale);
	}

}
