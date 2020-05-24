#pragma once

namespace InGame {

	enum WindowSelectorState : unsigned int
	{
		OnNothing,
		OnDropSelect,
		OnUtilSelect
	};

	enum WindowSelectorStat : unsigned int
	{
		FromID,
		ItemNum
	};

	enum WindowSelectorStatusHandleType : unsigned int
	{
		DrawSelectedPoint,
	};

}