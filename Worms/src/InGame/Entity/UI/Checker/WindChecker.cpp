#include "wmpch.h"
#include "WindChecker.h"


namespace InGame {

	WindChecker::WindChecker(const InitiateData& initData)
	{
		//Border
		m_Border = Gear::TextureStorage::GetTexture2D("WindCheckerBorder");
		float borderWidth = m_Border->GetWidth();
		float borderHeight = m_Border->GetHeight()* initData.WindowAspectRatio;

		glm::vec3 position( 0.8f, -0.83f, ZOrder::z_Checker);
		glm::vec3 scale(borderWidth / 1900.0f, borderHeight / 1900.0f, 1.0f);
		m_BorderTranslate = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);
	
		//Wind
		auto left = Gear::TextureStorage::GetFrameTexture2D("WindLeft");
		float leftWidth = left->GetWidth();
		float leftHeight = left->GetHeight();

		glm::vec3 leftPosition(0.707f, -0.83f, ZOrder::z_CheckerImg);
		glm::vec3 leftScale(borderWidth / 3700.0f, borderHeight / 2500.0f, 1.0f);
		m_LeftTranslate = glm::translate(glm::mat4(1.0f), leftPosition) * glm::scale(glm::mat4(1.0f), leftScale);
		m_LeftWind = Gear::Animation2D::Create(left, 0.02f, true);

		auto right = Gear::TextureStorage::GetFrameTexture2D("WindRight");
		glm::vec3 rightPosition(0.895f, -0.83f, ZOrder::z_CheckerImg);
		m_RightTranslate = glm::translate(glm::mat4(1.0f), rightPosition) * glm::scale(glm::mat4(1.0f), leftScale);
		m_RightWind = Gear::Animation2D::Create(right, 0.02f, true);

		m_RightWind->Start();
		m_LeftWind->Start();

		//Mask
		m_LeftMask = Gear::TextureStorage::GetTexture2D("WindMaskLeft");
		m_RightMask = Gear::TextureStorage::GetTexture2D("WindMaskRight");


	}

	void WindChecker::Render(Gear::Timestep ts)
	{
		m_LeftWind->Update(ts);
		m_RightWind->Update(ts);

		Gear::Renderer2D::DrawFixedQuad(m_BorderTranslate, m_Border);
		Gear::Renderer2D::DrawFixedAnimation(m_LeftTranslate, m_LeftWind);
		Gear::Renderer2D::DrawFixedAnimation(m_RightTranslate, m_RightWind);
	}

	WindChecker::~WindChecker()
	{
	}

}