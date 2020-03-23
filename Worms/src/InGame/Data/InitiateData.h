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

	namespace ZOrder {

		const float z_Grad = -0.8f;
		const float z_WatterBackGround = -0.7f;
		const float z_FloatingMatter = -0.6f;
		const float z_TerrainBack = -0.55f;
		const float z_WaterWaveB = -0.5f;
		const float z_CloudLarge = -0.38f;
		const float z_CloudMiddle = -0.35f;
		const float z_CloudSmall = -0.32f;
		const float z_Terrain = -0.1f;
		const float z_TerrainMask = -0.1f;
		const float z_Object = 0.0f;
		const float z_Worm = 0.1f;
		const float z_WaterWaveF = 0.2f;
		const float z_ItemSelect = 0.8f;
		const float z_ExitMenu = 0.85f;
		const float z_Cursor = 0.9f;
	}

	struct InitiateData
	{
		InitiateData() = default;

		float WindowWidth = Gear::WINDOW_WIDTH;
		float WindowHeight = Gear::WINDOW_HEIGHT;
		int nWorm = 3;
		int WormHP = 100;

		MapInfo Mapinfo;
		glm::vec3 MapPosition = {0.0f, -3.0f, ZOrder::z_Terrain};
		float MapReductionRatio = 37.0f;
		float WindMax = 30.0f;
		float CameraSliding = 0.9f;

		Gear::FRect WorldRect = { -70.0f, 70.0f, 70.0f, -27.0f };
		Gear::FRect CameraLimit = { -50.0f, 50.0f, 50.0f, -7.0f };
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
