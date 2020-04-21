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
		float TerrainMaxX;
		float TerrainMinX;
	};

	namespace WormInfo {

		enum Stat : unsigned int
		{
			Name,
			TeamName,
			TeamColor,
			Hp,
			SelectedItem,
			FireAngle,
			FirePower,
			NameBorderOffset,
			HpBorderOffset,
			ZRenderOffset,
			Direction,
			MoveSpeed,
			MyTurn,
			ItemExplosionTime,
			SelfDamage,
			Damage,
			UsedItem,
			Surrendered,
			TurnPassed,
			StatEnd
		};
		
		enum DirectionType : unsigned int
		{
			LeftFlat,
			RightFlat,
			LeftUp,
			RightUp,
			LeftDown,
			RightDown
		};
	}

	namespace TeamColor {

		enum Color : unsigned int
		{
			Red = WormInfo::Stat::StatEnd,
			Blue,
		};
	}

	namespace GraveInfo {

		enum Type : unsigned int
		{
			_1,
			_2,
			_3,
			_4,
			_5,
			_6
		};

		enum State : unsigned int
		{
			OnNothing,
			OnUnderWater
		};
	}

	struct WormSpecific
	{
		std::string Name;
		glm::vec3 StartPosition;
		GraveInfo::Type GraveType;
		int Hp;
		WormInfo::DirectionType Direction;
		float AdditionalZRenderOffset;
	};

	namespace ItemInfo {

		enum Type : unsigned int
		{
			Util, F1, F2, F3, F4, F5, F6,
			F7, F8, F9, F10, F11, F12
		};

		enum Number : unsigned int
		{
			JetPack, LowGravity, FastMove, ShowAim, Invisible,
			Bazooka, HomingMissile, Mortar, HomingPigeon, SheepLauncher,
			Grenade, ClusterBomb, Banana, BattleAxe, Earthquake,
			Shotgun, Handgun, Uzi, Minigun, Longbow,
			FirePunch, DragonBall, KamiKaze, Suicide, Prod,
			Dynamite, Mine, Sheep, SuperSheep, MoleBomb,
			AirStrike, NapalmStirke, MailStrike, MineStrike, MoleStrike,
			BlowTorch, PneumaticDril, Girder, BaseballBat, Girders,
			NinjaRope, Bungee, Parachute, Teleport, Scales,
			SupperBanana, HollyGrenade, FlameThrower, Tambourine, RiddleBomb,
			PetrolBomb, Skunk, Ceramics, FireShipStrike, CarpetStrike,
			MadCow, OldWoman, Dunkey, Sanctuary, Ammageddon,
			SkipGo, Surrender, TurnChange, Freeze, MagicMissile,
			BabyBanana, ItemEnd
		};

		struct ItemDescprition
		{
			ItemDescprition(Number number, const std::string& name, int quantity, int remainTurn)
				: ItemNumber(number), Name(name), Quantity(quantity), RemainTurn(remainTurn)
			{
				Texture = Gear::TextureStorage::GetTexture2D(Name);
			}

			Number ItemNumber;
			Gear::Ref<Gear::Texture2D> Texture;
			Gear::Util::FRect collisionRect;
			std::string Name;
			glm::mat4 Translate;
			int Quantity;
			int RemainTurn;
		};
	}

	struct TeamInfo
	{
		int nWorm = 0;
		int CurrentTotalWormHp = 0;
		int TotalWormHp = 0;
		std::string TeamName;
		TeamColor::Color TeamColor;
		std::vector<WormSpecific> worms;
		std::vector<ItemInfo::ItemDescprition> TeamItem;
		Gear::Ref<Gear::Texture2D> TeamIcon;
	};
	

	struct InitiateData
	{
		InitiateData() = default;

		float WindowWidth = Gear::WINDOW_WIDTH;
		float WindowHeight = Gear::WINDOW_HEIGHT;
		float WindowAspectRatio = Gear::WINDOW_WIDTH / (float)Gear::WINDOW_HEIGHT;
		
		float LimitTurnTime = 45.0f;
		int LimitSuddenDeathTurn = 10;
		
		int WormMaxHP = 100;

		MapInfo Mapinfo;
		float WormMoveSpeed = 1.0f;
		glm::vec3 MapPosition = {0.0f, -3.0f, ZOrder::z_Terrain};
		glm::vec2 WormScale = { 1.8f, 1.8f };
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
			ret.TerrainMaxX = 25.3f;
			ret.TerrainMinX = -25.3f;
			find = true;
		}

		GR_ASSERT(find, "Map Loading Fail!, There is no {0} Map", name);
		return ret;
	}
}

