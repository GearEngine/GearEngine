#pragma once

#include "Gear.h"
#include "Common/GameType.h"
#include "Common/EventChannel.h"
#include "Common/Font.h"
#include "imgui/imgui.h"

#include "InGame/Data/InitiateData.h"
#include "InGame/Data/EventData.h"

struct s
{
	bool turn = false;
};

inline void Some() {

	std::vector<s> v;


	bool BigBool = false;
	for (int i = 0; i < v.size(); ++i)
	{
		if (v[i].turn == true)
		{
			BigBool = true;
		}
	}

}