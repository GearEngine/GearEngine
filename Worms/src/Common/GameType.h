#pragma once

namespace ZOrder {

	const float z_FlatMouse = -0.93f;
	const float z_FlatFont = -0.92f;
	const float z_FlatCheckerImg = -0.91f;
	const float z_FlatChecker = -0.9f;
	const float z_Grad = -0.8f;
	const float z_FloatingMatter = -0.6f;
	const float z_WatterBackGround = -0.7f;
	const float z_TerrainBack = -0.55f;
	const float z_WaterWaveB = -0.5f;
	const float z_CloudLarge = -0.38f;
	const float z_CloudMiddle = -0.35f;
	const float z_CloudSmall = -0.32f;
	const float z_Terrain = -0.1f;
	const float z_TerrainMask = -0.1f;
	const float z_Object = 0.0f;
	const float z_ItemSelect = 0.1f;
	const float z_ExitMenu = 0.2f;
	const float z_Checker = 0.3f;
	const float z_CheckerImg = 0.31f;
	const float z_Font = 0.32f;
	const float z_Item = 0.5f;
	const float z_Grave = 0.65f;
	const float z_Worm = 0.7f;
	const float z_Blob = 0.71f;
	const float z_Smoke = 0.7108f;
	const float z_Flame = 0.7109f;
	const float z_ExplosionCircle = 0.711f;
	const float z_ExplosionEllipse = 0.712f;
	const float z_ExplosionSmoke = 0.713f;
	const float z_ExplosionText = 0.714f;
	const float z_Exhaust = 0.715f;
	const float z_FollowArrow = 0.75f;
	const float z_Aim = 0.76f;
	const float z_WaterWaveF = 0.8f;
	const float z_Cursor = 0.99f;

}

namespace BasicOption {

	enum Options : unsigned int
	{
		TurnTime,
		RoundTime,
		WinRequires,
		WormSelect,
		WormEnergy,
		Teleportin,
		OptionMax
	};

	inline int GetVaritaionCount(unsigned int type)
	{
		switch (type)
		{
		case BasicOption::TurnTime:
			return 6;
		case BasicOption::RoundTime:
			return 6;
		case BasicOption::WinRequires:
			return 6;
		case BasicOption::WormSelect:
			return 3;
		case BasicOption::WormEnergy:
			return 3;
		case BasicOption::Teleportin:
			return 2;
		}
		return  0;
	}

	inline std::string GetOptionName(unsigned int type)
	{
		switch (type)
		{
		case BasicOption::TurnTime:
			return "TurnTime";
		case BasicOption::RoundTime:
			return "RoundTime";
		case BasicOption::WinRequires:
			return "WinRequires";
		case BasicOption::WormSelect:
			return "WormSelect";
		case BasicOption::WormEnergy:
			return "WormEnergy";
		case BasicOption::Teleportin:
			return "Teleportin";
		}
		return "";
	}

	enum RoundTime : unsigned int
	{
		RT5,
		RT10,
		RT15,
		RT20,
		RT25,
		RT30,
		RTMax
	};
	
	inline std::string GetRoundTimeName(unsigned int type)
	{
		switch (type)
		{
		case RT5:
			return "RoundTime5";
		case RT10:
			return "RoundTime10";
		case RT15:
			return "RoundTime15";
		case RT20:
			return "RoundTime20";
		case RT25:
			return "RoundTime25";
		case RT30:
			return "RoundTime30";
		}
		return 0;
	}

	inline int GetRoundTime(unsigned int type)
	{
		switch (type)
		{
		case RT5:
			return 5;
		case RT10:
			return 10;
		case RT15:
			return 15;
		case RT20:
			return 20;
		case RT25:
			return 25;
		case RT30:
			return 30;
		}
		return 0;
	}

	enum Teleportin
	{
		On, Off
	};

	inline std::string GetTeleportinName(unsigned int type)
	{
		switch (type)
		{
		case On:
			return "TeleportinOn";
		case Off:
			return "TeleportinOff";
		}
		return 0;
	}

	enum TurnTime : unsigned int
	{
		TT15,
		TT20,
		TT30,
		TT45,
		TT60,
		TT90,
		TTMax
	};

	inline std::string GetTurnTimeName(unsigned int type)
	{
		switch (type)
		{
		case TT15:
			return "TurnTime15";
		case TT20:
			return "TurnTime20";
		case TT30:
			return "TurnTime30";
		case TT45:
			return "TurnTime45";
		case TT60:
			return "TurnTime60";
		case TT90:
			return "TurnTime90";
		}
		return 0;
	}

	inline int GetTurnTime(unsigned int type)
	{
		switch (type)
		{
		case TT15:
			return 15;
		case TT20:
			return 20;
		case TT30:
			return 30;
		case TT45:
			return 45;
		case TT60:
			return 60;
		case TT90:
			return 90;
		}
		return 0;
	}

	enum WinRequires
	{
		WR1,
		WR2,
		WR3,
		WR5,
		WR7,
		WR9,
		WRMax
	};

	inline std::string GetWinRequiresName(unsigned int type)
	{
		switch (type)
		{
		case WR1:
			return "WinRequires1";
		case WR2:
			return "WinRequires2";
		case WR3:
			return "WinRequires3";
		case WR5:
			return "WinRequires5";
		case WR7:
			return "WinRequires7";
		case WR9:
			return "WinRequires9";
		}
		return 0;
	}

	inline int GetWinRequires(unsigned int type)
	{
		switch (type)
		{
		case WR1:
			return 1;
		case WR2:
			return 2;
		case WR3:
			return 3;
		case WR5:
			return 5;
		case WR7:
			return 7;
		case WR9:
			return 9;
		}
		return 0;
	}

	enum WormEnergy
	{
		WE100,
		WE150,
		WE200,
		WEMax
	};

	inline std::string GetWormEnergyName(unsigned int type)
	{
		switch (type)
		{
		case WE100:
			return "WormEnergy100";
		case WE150:
			return "WormEnergy150";
		case WE200:
			return "WormEnergy200";
		}
		return 0;
	}

	inline int GetWormEnergy(unsigned int type)
	{
		switch (type)
		{
		case WE100:
			return 100;
		case WE150:
			return 150;
		case WE200:
			return 200;
		}
		return 0;
	}

	enum WormSelect
	{
		WSOn,
		WSOff,
		WSRnd
	};

	inline std::string GetWormSelectName(unsigned int type)
	{
		switch (type)
		{
		case WSOn:
			return "WormSelectOn";
		case WSOff:
			return "WormSelectOff";
		case WSRnd:
			return "WormSelectRandom";
		}
	}

	inline std::string GetVariationName(unsigned int type, unsigned int variation)
	{
		switch (type)
		{
		case BasicOption::TurnTime:
			return GetTurnTimeName(variation);
		case BasicOption::RoundTime:
			return GetRoundTimeName(variation);
		case BasicOption::WinRequires:
			return GetWinRequiresName(variation);
		case BasicOption::WormSelect:
			return GetWormSelectName(variation);
		case BasicOption::WormEnergy:
			return GetWormEnergyName(variation);
		case BasicOption::Teleportin:
			return GetTeleportinName(variation);
		}
	}

}

namespace TeamBasicOption
{
	enum WormCount : unsigned int
	{
		_1, 
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		Max
	};

	enum Handicap : unsigned int
	{
		Plus,
		Minus,
		None,
		HMax,
	};

	enum Ally : unsigned int
	{
		a_1,
		a_2,
		a_3,
		a_4,
		a_5,
		a_6,
		AMax
	};

	inline std::string GetHandicapName(unsigned int type)
	{
		switch (type)
		{
		case TeamBasicOption::Plus:
			return "Plus";
		case TeamBasicOption::Minus:
			return "Minus";
		case TeamBasicOption::None:
			return "None";
		}
		return "";
	}
}

namespace PlayerType
{
	enum Type : unsigned int
	{
		Human,
		CPU1,
		CPU2,
		CPU3,
		CPU4,
		CPU5,
		Max
	};

	inline std::string GetName(unsigned int type)
	{
		switch (type)
		{
		case PlayerType::Human:
			return "Human";
		case PlayerType::CPU1:
			return "CPU1";
		case PlayerType::CPU2:
			return "CPU2";
		case PlayerType::CPU3:
			return "CPU3";
		case PlayerType::CPU4:
			return "CPU4";
		case PlayerType::CPU5:
			return "CPU5";
		}
		return "";
	}
}

namespace Map {

	const int MapMax = 33;

	enum Type : unsigned int
	{
		Books,
		Chateau,
		Chemist,
		City,
		Desert,
		Dino,
		Droids,
		Farmyard,
		Flowers,
		Fruity2,
		Horny,
		Kermit,
		MadCows,
		Mars,
		MPSLogo,
		Mushroom,
		Paris,
		Plumbing,
		Polar,
		Pumpkins,
		Rocky,
		Shoes,
		Snow,
		Speccy,
		Sub,
		T17Space,
		Tech,
		Toys,
		Train,
		Woods,
		Wormore,
		Max
	};

	inline std::string GetName(unsigned int type)
	{
		switch (type)
		{
		case Map::Books:
			return "Books";
		case Map::Chateau:
			return "Chateau";
		case Map::Chemist:
			return "Chemist";
		case Map::City:
			return "City";
		case Map::Desert:
			return "Desert";
		case Map::Dino:
			return "Dino";
		case Map::Droids:
			return "Droids";
		case Map::Farmyard:
			return "Farmyard";
		case Map::Flowers:
			return "Flowers";
		case Map::Fruity2:
			return "Fruity2";
		case Map::Horny:
			return "Horny";
		case Map::Kermit:
			return "Kermit";
		case Map::MadCows:
			return "MadCows";
		case Map::Mars:
			return "Mars";
		case Map::MPSLogo:
			return "MPSLogo";
		case Map::Mushroom:
			return "Mushroom";
		case Map::Paris:
			return "Paris";
		case Map::Plumbing:
			return "Plumbing";
		case Map::Polar:
			return "Polar";
		case Map::Pumpkins:
			return "Pumpkins";
		case Map::Rocky:
			return "Rocky";
		case Map::Shoes:
			return "Shoes";
		case Map::Snow:
			return "Snow";
		case Map::Speccy:
			return "Speccy";
		case Map::Sub:
			return "Sub";
		case Map::T17Space:
			return "T17Space";
		case Map::Tech:
			return "Tech";
		case Map::Toys:
			return "Toys";
		case Map::Train:
			return "Train";
		case Map::Woods:
			return "Woods";
		case Map::Wormore:
			return "Wormore";
		}
		return "";
	}
}

namespace Main_Multi {

	enum Ready : unsigned int
	{
		TurnTime,
		RoundTime,
		WinRequires,
		WormSelect,
		WormEnergy,
		Teleportin,
		Map,
		MapSelector,
		MapSelectorList,
		Teams,
		WeaponsOption,
		GameOption,
		GameSchemeSelector,
		Delete,
		Save,
		Barraks,
		TeamName,
		Points,
		CreateNewTeam,
		StartGame,
		Exit,
		Max
	};
}

namespace GameMode {
	 
	enum Type : unsigned int
	{
		Multi, 
		NetWork,
	};

	static unsigned int Mode;
}

namespace SceneBackground {

	static Gear::OrthographicCamera UiCamera(-1.7777f, 1.7777f, -1.0f, 1.0f);
	const int MaxStar = 30;

	struct StarInfo
	{
		StarInfo()
		{
			Transform = glm::translate(glm::mat4(1.0f), glm::vec3(Gear::Util::GetRndFloatFromTo(-1.28f, 1.37f), Gear::Util::GetRndFloatFromTo(-1.0f, 0.98f), 0.1f))
				* glm::scale(glm::mat4(1.0f), glm::vec3(0.12444444f, 0.07f, 1.0f));

			Index = Gear::Util::GetRndInt(128);

			dx = Gear::Util::GetRndFloatFromTo(0.01f, 0.015f);
			dy = 0.023f - dx;
		}

		void Update(Gear::Timestep ts)
		{
			Transform[3][0] += dx;
			Transform[3][1] -= dy;

			if (Transform[3][0] >= 1.3f)
			{
				Transform[3][0] = -1.27;
			}
			if (Transform[3][0] <= -1.28f)
			{
				Transform[3][0] = 1.3f;
			}
			if (Transform[3][1] >= 0.98f)
			{
				Transform[3][1] = -1.0f;
			}
			if (Transform[3][1] <= -1.0f)
			{
				Transform[3][1] = 0.98f;
			}

			pastTime += ts;
			if (pastTime >= pastTime)
			{
				pastTime = 0.0f;
				++Index;
				if (Index >= 129)
				{
					Index = 0;
				}
			}
		}

		float pastTime = 0.0f;
		float frameDelay = 0.1f;
		float dx;
		float dy;
		int Index;
		glm::mat4 Transform;
	};
}

namespace WormsSound
{
	enum Channel : unsigned int
	{
		bgm, effect, wormSpeech, wormWalk, wormAct, Weapon, Water, Grave, Mouse
	};

}