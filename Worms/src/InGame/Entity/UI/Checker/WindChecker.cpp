#include "wmpch.h"
#include "WindChecker.h"

#include "InGame/Layer/BackGroundLayer.h"

namespace InGame {

	WindChecker::WindChecker(const InitiateData& initData)
	{
		//Border
		m_Border = Gear::TextureStorage::GetTexture2D("WindCheckerBorder");
		float borderWidth = m_Border->GetWidth();
		float borderHeight = m_Border->GetHeight()* initData.WindowAspectRatio;

		glm::vec3 position( 0.8f, -0.83f, ZOrder::z_FlatChecker);
		glm::vec3 scale(borderWidth / 1900.0f, borderHeight / 1900.0f, 1.0f);
		m_BorderTranslate = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);
	
		//Wind
		auto left = Gear::TextureStorage::GetFrameTexture2D("WindLeft");
		float leftWidth = left->GetWidth();
		float leftHeight = left->GetHeight();

		glm::vec3 leftPosition(0.711f, -0.83f, ZOrder::z_FlatCheckerImg);
		glm::vec3 leftScale(borderWidth / 4000.0f, borderHeight / 2500.0f, 1.0f);
		m_LeftTranslate = glm::translate(glm::mat4(1.0f), leftPosition) * glm::scale(glm::mat4(1.0f), leftScale);
		m_LeftWind = Gear::Animation2D::Create(left, 0.02f, true);

		auto right = Gear::TextureStorage::GetFrameTexture2D("WindRight");
		glm::vec3 rightPosition(0.889f, -0.83f, ZOrder::z_FlatCheckerImg);
		m_RightTranslate = glm::translate(glm::mat4(1.0f), rightPosition) * glm::scale(glm::mat4(1.0f), leftScale);
		m_RightWind = Gear::Animation2D::Create(right, 0.02f, true);

		m_RightWind->Start();
		m_LeftWind->Start();

		//Mask
		m_Mask = Gear::TextureStorage::GetTexture2D("WindMask");
		m_WindMax = initData.WindMax;
		m_CurrentWind = 0.0f;

		int maskWidth = m_Mask->GetWidth();
		int maskHeight = m_Mask->GetHeight();
		m_maskWidthUnit = maskWidth / 550.0f;
		m_maskHeightUnit = maskHeight / 270.0f;

		glm::vec3 maskScale(m_maskWidthUnit, m_maskHeightUnit, 1.0f);
		glm::vec3 leftMaskPosition(m_LeftMaskBasis, -0.83f, ZOrder::z_FlatFont);
		glm::vec3 rightMaskPosition(m_RightMaskBasis, -0.83f, ZOrder::z_FlatFont);

		m_BasicLeftMaskTranslate = glm::translate(glm::mat4(1.0f), leftMaskPosition) * glm::scale(glm::mat4(1.0f), maskScale);
		m_BasicRightMaskTranslate = glm::translate(glm::mat4(1.0f), rightMaskPosition) * glm::scale(glm::mat4(1.0f), maskScale);
	
		m_LeftMaskTranslate = m_BasicLeftMaskTranslate;
		m_RightMaskTranslate = m_BasicRightMaskTranslate;
	}

	void WindChecker::Render(Gear::Timestep ts)
	{
		m_LeftWind->Update(ts);
		m_RightWind->Update(ts);

		float currentWind = BackGroundLayer::GetCurrentWind();
		if (currentWind != m_CurrentWind)
		{
			m_CurrentWind = currentWind;
			LeftWindMaskUpdate();
			RightWindMaskUpdate();
		}


		Gear::Renderer2D::DrawFixedQuad(m_BorderTranslate, m_Border);
		Gear::Renderer2D::DrawFixedAnimation(m_LeftTranslate, m_LeftWind);
		Gear::Renderer2D::DrawFixedAnimation(m_RightTranslate, m_RightWind);

		Gear::Renderer2D::DrawFixedQuad(m_LeftMaskTranslate, m_Mask);
		Gear::Renderer2D::DrawFixedQuad(m_RightMaskTranslate, m_Mask);
	}

	void WindChecker::LeftWindMaskUpdate()
	{
		if (m_CurrentWind < 0.0f)
		{
			m_LeftMaskTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(m_LeftMaskBasis + 0.088f * m_CurrentWind / m_WindMax, -0.83f, ZOrder::z_FlatFont)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(m_maskWidthUnit * (1.0f + m_CurrentWind / m_WindMax), m_maskHeightUnit, 1.0f));

		}
		else
		{
			m_LeftMaskTranslate = m_BasicLeftMaskTranslate;
		}

	}

	void WindChecker::RightWindMaskUpdate()
	{
		if (m_CurrentWind > 0.0f)
		{
			m_RightMaskTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(m_RightMaskBasis + 0.088f * m_CurrentWind / m_WindMax, -0.83f, ZOrder::z_FlatFont)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(m_maskWidthUnit * (1.0f - m_CurrentWind / m_WindMax), m_maskHeightUnit, 1.0f));

		}
		else
		{
			m_RightMaskTranslate = m_BasicRightMaskTranslate;
		}
	}

	WindChecker::~WindChecker()
	{
	}

}