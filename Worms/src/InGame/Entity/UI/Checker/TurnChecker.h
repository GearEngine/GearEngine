#pragma once

namespace InGame {

	class TurnChecker 
	{
	public:
		TurnChecker(const InitiateData& initData);
		~TurnChecker();

		void Render();

	private:
		glm::mat4 m_Translate;
		Gear::Ref<Gear::Texture2D> m_Border;
		glm::vec3 m_FontScale;
	};

}