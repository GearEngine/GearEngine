#pragma once

namespace InGame {

	class Mouse 
	{
	public:
		Mouse(const InitiateData& initData);
		~Mouse();

		void OnUpdate(Gear::Timestep ts);

	private:
		int m_ID;

		std::pair<float, float> m_CurrentPosition;
		std::pair<float, float> m_MiddlePosition;
		std::pair<float, float> m_QuitPosition;
		std::pair<float, float> m_ItemSelectPosition;
	};




}