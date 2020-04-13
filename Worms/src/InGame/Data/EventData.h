#pragma once
#include "InGame/Entity/Object/Effects/Effects.h"

namespace InGame {

	namespace EventType
	{
		enum : unsigned int
		{
			World,
			Explosion,
			WormMove,
			MouseMove,
			MouseClick,
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
		GameEnd
	};

	struct ExplosionData
	{
		ExplosionData(const glm::vec2 position, Explosion::Size size, Explosion::Text text)
			: Position(position), Size(size), Text(text)
		{
		}
		glm::vec2 Position;

		Explosion::Size Size;
		Explosion::Text Text;
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

	struct DamageData
	{
		DamageData(int damage, const std::string& teamName)
			:Damage(damage), WormTeamName(teamName)
		{}

		int Damage;
		std::string WormTeamName;
	};

}

