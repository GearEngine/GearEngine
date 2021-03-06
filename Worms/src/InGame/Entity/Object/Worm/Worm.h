#pragma once

#include "WormStatusHandler.h"

namespace InGame {

	class Worm
	{
	public:
		Worm(int teamNumber, int wormNumber, const InitiateData& initData);
		~Worm();

		void Update(Gear::Timestep ts);
		int GetID() { return m_ID; }

	private:
		void InitWormPosition(WormSpecific& wormData, int wormNumber, int teamNumber, Gear::Ref<Gear::Texture2D> mask, const glm::mat4& maskTranslate, int& fixedBottomPos, int& wormOnTexturePositionX);
		void GradientCheck(int midX, int bottomY, Gear::Ref<Gear::Texture2D> mask, WormInfo::DirectionType dir);

		int m_ID;

	};
}
