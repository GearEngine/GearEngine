#pragma once

namespace InGame {

	class WindChecker
	{
	public:
		WindChecker(const InitiateData& initData);
		~WindChecker();
		void Render(Gear::Timestep ts);

	private:
		void LeftWindMaskUpdate();
		void RightWindMaskUpdate();

	private:
		glm::mat4 m_BorderTranslate;
		glm::mat4 m_LeftTranslate;
		glm::mat4 m_LeftMaskTranslate;
		glm::mat4 m_RightTranslate;
		glm::mat4 m_RightMaskTranslate;

		glm::mat4 m_BasicRightMaskTranslate;
		glm::mat4 m_BasicLeftMaskTranslate;

		float m_LeftMaskBasis = 0.711f;
		float m_RightMaskBasis = 0.889f;
		float WindYPost = -0.95f;
		float m_maskWidthUnit;
		float m_maskHeightUnit;
		float m_WindMax;
		float m_CurrentWind;

		Gear::Ref<Gear::Texture2D> m_Border;
		Gear::Ref<Gear::Animation2D> m_RightWind;
		Gear::Ref<Gear::Animation2D> m_LeftWind;
		Gear::Ref<Gear::Texture2D> m_Mask;
	};

}