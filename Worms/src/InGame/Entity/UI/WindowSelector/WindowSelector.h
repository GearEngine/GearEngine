#pragma once

#include "WindowSelectorFSM.h"

namespace InGame {

	class WindowSelector
	{
	public:
		WindowSelector(const InitiateData& initData);
		~WindowSelector();

	private:
		int m_ID;
	};
}

