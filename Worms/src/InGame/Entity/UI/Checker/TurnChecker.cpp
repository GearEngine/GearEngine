#include "wmpch.h"
#include "TurnChecker.h"

#include "InGame/Entity/World/World.h"

namespace InGame {

	TurnChecker::TurnChecker(const InitiateData & initData)
	{
		m_Border = Gear::TextureStorage::GetTexture2D("TurnCheckerBorder");
		float width = m_Border->GetWidth();
		float height = m_Border->GetHeight()* initData.WindowAspectRatio;

		glm::vec3 position(-0.91f, -0.71f, ZOrder::z_Checker);
		glm::vec3 scale(width / 1850.0f, height / 1850.0f, 1.0f);

		m_Translate = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), scale);
		m_FontScale = glm::vec3(0.009f * 3, 0.02f * 3, ZOrder::z_Font);
	}

	TurnChecker::~TurnChecker()
	{
	}

	void TurnChecker::Render()
	{
		int turn = World::s_CurrentTurn;
		std::string str = "00:";
		if (turn < 10)
		{
			str += "0" + std::to_string(turn);
		}
		else
		{
			str += std::to_string(turn);
		}
		Font::printFont(glm::vec2(-0.903f, -0.709f), m_FontScale, str, FontType::GraySmall, 0.015f);
		Gear::Renderer2D::DrawFixedQuad(m_Translate, m_Border);
	}

}