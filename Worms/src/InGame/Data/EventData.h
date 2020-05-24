#pragma once
#include "InGame/Entity/Object/Effects/Effects.h"

namespace InGame {

	namespace EventType
	{
		enum : unsigned int
		{
			World,
			Explosion,
			Worm,
			MouseMove,
			MouseClick,
			Timer,
			Turn
		};
	}

	enum WorldDataType : unsigned int
	{
		CreatedWorm,
		NewStart,
		OnExitMenu,
		ReFollow,
		NewFollow,
		PrepareNextPhase,
		TurnOver,
		RunningStart,
		DamageWorm,
		GetDamageWorm,
		WormDie,
		TeamInfoBlinkOff,
		Surrender,
		GameEnd
	};

	enum WindowSelctorType : unsigned int 
	{
		Drop,
		Util,
		Stop
	};

	struct WindowSelectData
	{
		WindowSelectData(int fromID, int type, ItemInfo::Number itemNum)
			: FromID(fromID), Type(type), ItemNum(itemNum)
		{}

		int FromID;
		int Type;
		ItemInfo::Number ItemNum;
	};

	struct ExplosionData
	{
		ExplosionData(const glm::vec2 position, Explosion::Size size, float power, unsigned int itemNumber = 0xffffffffu, int fromEntityID = -1)
			: Position(position), Size(size), Power(power), ItemNumber(itemNumber), FromEntityID(fromEntityID)
		{}

		glm::vec2 Position;
		int FromEntityID;
		float Power;
		unsigned int ItemNumber;
		Explosion::Size Size;
	};

	struct MouseMoveData
	{
		MouseMoveData(float _dx, float _dy)
			: dx(_dx), dy(_dy)
		{}
		float dx, dy;
	};

	struct WorldData
	{
		WorldData(WorldDataType dataType, const std::any& data = 0, int entityID = -1)
			: DataType(dataType), Data(data), EntityID(entityID)
		{}

		WorldDataType DataType;
		std::any Data;
		int EntityID;
	};

	struct UseItemData
	{
		UseItemData(unsigned int itemNumber, const std::string& teamName)
			: ItemNumber(itemNumber), TeamName(teamName)
		{
		}

		unsigned int ItemNumber;
		std::string TeamName;
	};

	struct DamageData
	{
		DamageData(int damage, const std::string& teamName)
			:Damage(damage), WormTeamName(teamName)
		{}

		int Damage;
		std::string WormTeamName;
	};

	struct WorldMassage
	{
		WorldMassage() = default;

		WorldMassage(const std::string& str, FontType::Type type)
			: Str(str), Type(type)
		{}

		FontType::Type Type;
		std::string Str;
	};

}

