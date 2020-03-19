#pragma once
#include "InGame/Data/InitiateData.h"

namespace InGame {

	class Mouse 
	{
	public:
		Mouse(const InitiateData& initData);
		~Mouse();

		void OnUpdate(Gear::Timestep ts);

	private:
		int m_ID;

		void* m_Window;
		std::pair<float, float> m_CurrentPosition;
		std::pair<float, float> m_MiddlePosition;
		std::pair<float, float> m_QuitPosition;
		std::pair<float, float> m_ItemSelectPosition;
	};




}