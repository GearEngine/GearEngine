#pragma once

namespace InGame {

	class WindChecker
	{
	public:
		WindChecker(const InitiateData& initData);
		~WindChecker();

	public:
		void Render(Gear::Timestep ts);

	private:
		glm::mat4 m_BorderTranslate;
		glm::mat4 m_LeftTranslate;
		glm::mat4 m_LeftMaskTranslate;
		glm::mat4 m_RightTranslate;
		glm::mat4 m_RightMaskTranslate;

		Gear::Ref<Gear::Texture2D> m_Border;
		Gear::Ref<Gear::Animation2D> m_RightWind;
		Gear::Ref<Gear::Animation2D> m_LeftWind;
		Gear::Ref<Gear::Texture2D> m_LeftMask;
		Gear::Ref<Gear::Texture2D> m_RightMask;

	};


}