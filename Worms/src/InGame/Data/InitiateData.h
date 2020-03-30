#pragma once

namespace InGame {

	struct MapInfo
	{
		std::string MapName;
		std::string TerrianBackName;
		std::string FloatingMaterial;
		std::string Water;
		std::string Grad;
		glm::vec4 WaterRGB;
	};

	namespace WormInfo {

		enum Stat : unsigned int
		{
			Name,
			TeamName,
			TeamColor,
			Hp,
			SelectedItem,
			FireAngleVector,
			FirePower,
			NameBorderOffset,
			HpBorderOffset,
			ZRenderOffset,
			StatEnd
		};
		
	}

	namespace TeamColor {

		enum Color : unsigned int
		{
			Red = WormInfo::Stat::StatEnd,
			Blue,
		};
	}

	struct WormSpecific
	{
		std::string Name;
		glm::vec3 StartPosition;
		int Hp;
		float AdditionalZRenderOffset;
	};

	struct TeamInfo
	{
		int nWorm = 0;
		int CurrentTotalWormHp = 0;
		int TotalWormHp = 0;
		std::string TeamName;
		TeamColor::Color TeamColor;
		std::vector<WormSpecific> worms;
		Gear::Ref<Gear::Texture2D> TeamIcon;
	};

	struct InitiateData
	{
		InitiateData() = default;

		float WindowWidth = Gear::WINDOW_WIDTH;
		float WindowHeight = Gear::WINDOW_HEIGHT;
		float WindowAspectRatio = Gear::WINDOW_WIDTH / (float)Gear::WINDOW_HEIGHT;
		
		float LimitTurnTime = 15.0f;
		int LimitSuddenDeathTurn = 10;
		
		int WormMaxHP = 100;

		MapInfo Mapinfo;
		glm::vec3 MapPosition = {0.0f, -3.0f, ZOrder::z_Terrain};
		glm::vec2 WormScale = { 0.7f, 0.7f };
		float MapReductionRatio = 37.0f;
		int WindMax = 30;
		float CameraSliding = 0.9f;

		Gear::Util::FRect WorldRect = { -70.0f, 70.0f, 70.0f, -27.0f };
		Gear::Util::FRect CameraLimit = { -50.0f, 50.0f, 50.0f, -7.0f };

		std::vector<TeamInfo> Teams;
		int nTotalWorms = 0;
	};

	inline MapInfo GetMapInfo(std::string name)
	{
		MapInfo ret;
		bool find = false;

		if (name == "City")
		{
			ret.MapName = "City";
			ret.FloatingMaterial = "Leaf";
			ret.TerrianBackName = "BlueMountain";
			ret.Water = "BlueWater";
			ret.Grad = "Grad0";
			ret.WaterRGB = { 50 / 255.0f, 59 / 255.0f, 126 / 255.0f, 1.0f };
			find = true;
		}

		GR_ASSERT(find, "Map Loading Fail!, There is no {0} Map", name);
		return ret;
	}
}

