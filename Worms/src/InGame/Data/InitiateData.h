#pragma once

namespace InGame {

	class MapInfo : public Gear::JsonAble
	{
	public:
		Gear::Ref<Gear::Texture2D> Map;
		Gear::Ref<Gear::Texture2D> Mask;
		std::string MapName;
		std::string TerrianBackName;
		std::string FloatingMaterial;
		std::string Water;
		std::string Grad;
		glm::vec4 WaterRGB;
		float TerrainMaxX;
		float TerrainMinX;

		void MapLoad()
		{
			Map = Gear::Texture2D::Create("assets\\Terrain\\" + MapName + ".png");
			Mask = Gear::Texture2D::Create("assets\\Terrain\\" + MapName + "Mask.png");
		}

		virtual void Read(const Json::Value& value) override
		{
			Json::Value mapValue = value[Name];
			MapName = Name;
			TerrianBackName = mapValue["TerrianBackName"].asString();
			FloatingMaterial = mapValue["FloatingMaterial"].asString();
			Water = mapValue["Water"].asString();
			Grad = mapValue["Grad"].asString();
			WaterRGB = glm::vec4(mapValue["WR"].asFloat() / 255.0f, mapValue["WG"].asFloat() / 255.0f, mapValue["WB"].asFloat() / 255.0f, mapValue["WA"].asFloat() / 255.0f);
			TerrainMaxX = mapValue["TerrainMaxX"].asFloat();
			TerrainMinX = mapValue["TerrainMinX"].asFloat();
		}

		virtual void Write(Json::Value& value)
		{

		}

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
			Green,
			Yellow,
			Purple,
			Sky
		};

		inline Color ConvertAllyToColor(unsigned int type)
		{
			switch (type)
			{
			case 0:
				return Color::Red;
			case 1:
				return Color::Blue;
			case 2:
				return Color::Green;
			case 3:
				return Color::Yellow;
			case 4:
				return Color::Purple;
			case 5:
				return Color::Sky;
			}
		}
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
	

	struct InitiateData : public Gear::PacketAble
	{
		InitiateData() = default;

		int GameMode;

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

		virtual void Read(const Gear::InputMemoryStream& stream) override
		{

		}
		virtual void Write(Gear::OutputMemoryStream& stream) override
		{

		}
	};

	inline MapInfo GetMapInfo(const std::string& mapName)
	{
		MapInfo ret;
		ret.SetName(mapName);
		Gear::JsonManager::Get()->Read("assets\\Data\\MapInfo\\MapInfo.json", ret);
		return ret;
	}
}

