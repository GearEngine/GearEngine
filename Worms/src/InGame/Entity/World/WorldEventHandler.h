#pragma once

#include "WorldState.h"
#include "InGame/Entity/Object/Worm/WormEnum.h"
#include "Common/EventChannel.h"

namespace InGame {

	class WorldWormData
	{
	public: 
		static std::vector<int> s_LivingWorms;
		static std::vector<int> s_ActiveWorms;
		static std::queue<int> s_WaitingDyeQue;
	};

	struct WorldDenoteData
	{
		WorldDenoteData(const Gear::Ref<Gear::Texture2D>& waitingTimeBorder)
			: WaitingTimeBorder(waitingTimeBorder)
		{
			float width = waitingTimeBorder->GetWidth();
			float height = waitingTimeBorder->GetHeight();

			WaitingTimeBorderTranslate =
				glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.8f, ZOrder::z_FlatChecker))
				* glm::scale(glm::mat4(1.0f), glm::vec3(width / 1500.0f, height / 1500.0f, 1.0f));

			WaitingTimeFontPosition = glm::vec3(0.05f, 0.8f, ZOrder::z_FlatFont);
		}

		glm::mat4 WaitingTimeBorderTranslate;
		glm::vec3 WaitingTimeFontPosition;

		Gear::Ref<Gear::Texture2D> WaitingTimeBorder;
	};

	struct WorldTeamInfoDenoteData
	{
		WorldTeamInfoDenoteData() = default;
		WorldTeamInfoDenoteData(const Gear::Ref<Gear::Texture2D>& nameBorder)
			: NameBorder(nameBorder)
		{
			float width = nameBorder->GetWidth();
			float height = NameBorder->GetHeight();

			TeamNameBorderWidthUnit = width / 1700.0f;
			TeamNameBorderHeightUnit = height / 1200.0f;

			TeamInfoBorderTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(-0.092f, -0.94f, ZOrder::z_FlatChecker)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(TeamNameBorderWidthUnit, TeamNameBorderHeightUnit, 1.0f));

			TeamNameFontPosition = glm::vec3(-0.082f, -0.94f, ZOrder::z_FlatFont);
			yOffset = 0.06f;

			float iconWidth = 15;
			float iconHeight = 15;
			IconTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.94, ZOrder::z_FlatCheckerImg))
				* glm::scale(glm::mat4(1.0f), glm::vec3(iconWidth / 380.0f, iconHeight / 270.0f, 1.0f));

			HpBarBasisPositionX = 0.128f;
			HpBasisBarPosition = glm::vec3(HpBarBasisPositionX, -0.94f, ZOrder::z_FlatCheckerImg);

			HpBarWidthUnit = 104.0f / 500.0f;
			HpBarHeightUnit = 17.0f / 300.0f;

		}

		glm::mat4 TeamInfoBorderTranslate;
		glm::mat4 IconTranslate;
		glm::vec3 TeamNameFontPosition;
		glm::vec3 HpBasisBarPosition;

		float TeamNameBorderWidthUnit;
		float TeamNameBorderHeightUnit;

		float HpBarWidthUnit;
		float HpBarHeightUnit;
		float HpBarBasisPositionX;

		float yOffset;

		Gear::Ref<Gear::Texture2D> NameBorder;

	};

	class WorldEventHandler : public Gear::EventHandler
	{
		bool InFirst = true;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Timer> worldTimer;

		bool inDamageWormFirst = true;
		bool onShotgun = false;
		bool onDonkey = false;
		bool onHos = false;
		int curWorm;

		void init(int entityID)
		{
			status = Gear::EntitySystem::GetStatus(entityID);
			FSM = Gear::EntitySystem::GetFSM(entityID);
			worldTimer = Gear::EntitySystem::GetTimer(entityID);

			InFirst = false;
		}

		virtual void Handle(std::any data, int entityID, bool& handled) override;
	};
	
	
}