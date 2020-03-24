#pragma once

namespace InGame {

	class TurnChecker 
	{
	public:
		TurnChecker(const InitiateData& initData);
		~TurnChecker();

	private:
		int m_ID;

	};

}