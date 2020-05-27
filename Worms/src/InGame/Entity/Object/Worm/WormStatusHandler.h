#pragma once

#include "WormFSMHandler.h"

namespace InGame {

	struct WormDenoteData
	{
		WormDenoteData(float nameTextureWidthUnint, float nameTextureHeightUnit, const Gear::Ref<Gear::Texture2D>& nameBorder,
			float hpTextureWidthUnit, float hpTextureHeightUnit, const Gear::Ref<Gear::Texture2D> hpBorder)
			: NameTextureWidthUnit(nameTextureWidthUnint), NameTextureHeightUnit(nameTextureHeightUnit), NameBorder(nameBorder),
			HpTextureWidthUnit(hpTextureWidthUnit), HpTextureHeightUnit(hpTextureHeightUnit), HpBorder(hpBorder)
		{}
		float NameTextureWidthUnit;
		float NameTextureHeightUnit;
		float HpTextureWidthUnit;
		float HpTextureHeightUnit;
		Gear::Ref<Gear::Texture2D> NameBorder;
		Gear::Ref<Gear::Texture2D> HpBorder;
	};

	class WormDisplayHanlder : public Gear::Status::StatusHandler
	{
		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			//Hp
			auto& denoteData = std::any_cast<WormDenoteData>(data.Data);
			auto originPosition = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();
			glm::vec3 hpPosition = originPosition;
			glm::vec3 namePosition = originPosition;

			int hp = std::any_cast<int>(statlist[WormInfo::Stat::Hp]);
			float hpOffset = std::any_cast<float>(statlist[WormInfo::Stat::HpBorderOffset]);
			float additionalZOffset = std::any_cast<float>(statlist[WormInfo::Stat::ZRenderOffset]);
			TeamColor::Color teamColor = std::any_cast<TeamColor::Color>(statlist[WormInfo::TeamColor]);

			hpPosition.y += hpOffset;
			hpPosition.z = ZOrder::z_Checker + additionalZOffset;
			Gear::Renderer2D::DrawQuad(hpPosition, glm::vec2(denoteData.HpTextureWidthUnit, denoteData.HpTextureHeightUnit), denoteData.HpBorder);

			hpPosition.x += 0.15f;
			hpPosition.z = ZOrder::z_Font + additionalZOffset;

			//Name
			std::string name = std::any_cast<std::string>(statlist[WormInfo::Stat::Name]);
			float nameOffset = std::any_cast<float>(statlist[WormInfo::Stat::NameBorderOffset]);
			int strlen = name.length();

			namePosition.y += nameOffset;
			namePosition.z = ZOrder::z_Checker + additionalZOffset;
			Gear::Renderer2D::DrawQuad(namePosition, glm::vec2(denoteData.NameTextureWidthUnit * strlen * 0.16f, denoteData.NameTextureHeightUnit), denoteData.NameBorder);

			namePosition.x += 0.15f;
			namePosition.y += 0.04f;
			namePosition.z = ZOrder::z_Font + additionalZOffset;

			switch (teamColor)
			{
			case InGame::TeamColor::Red:
				Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::RedSmall, 0.3f, false);
				Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::RedSmall, 0.3f, false);
				break;
			case InGame::TeamColor::Blue:
				Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::BlueSmall, 0.3f, false);
				Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::BlueSmall, 0.3f, false);
				break;
			case InGame::TeamColor::Green:
				Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::GreenSmall, 0.3f, false);
				Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::GreenSmall, 0.3f, false);
				break;
			case InGame::TeamColor::Yellow:
				Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::YellowSmall, 0.3f, false);
				Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::YellowSmall, 0.3f, false);
				break;
			case InGame::TeamColor::Purple:
				Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::PurpleSmall, 0.3f, false);
				Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::PurpleSmall, 0.3f, false);
				break;
			case InGame::TeamColor::Sky:
				Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::SkySmall, 0.3f, false);
				Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::SkySmall, 0.3f, false);
				break;
			}
		}
	};

	class WormWaitingDisplayHanlder : public Gear::Status::StatusHandler
	{
		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			//time check

			static const float blinkDelay = 0.5f;
			static float elapsedTime = 0.0f;
			static bool onWhite = false;
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			elapsedTime += timer->GetTick();
			if (blinkDelay < elapsedTime)
			{
				elapsedTime = 0.0f;
				onWhite = !onWhite;
			}

			//Hp
			auto& denoteData = std::any_cast<WormDenoteData>(data.Data);
			auto originPosition = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();
			glm::vec3 hpPosition = originPosition;

			int hp = std::any_cast<int>(statlist[WormInfo::Stat::Hp]);
			float hpOffset = std::any_cast<float>(statlist[WormInfo::Stat::HpBorderOffset]);
			float additionalZOffset = std::any_cast<float>(statlist[WormInfo::Stat::ZRenderOffset]);
			TeamColor::Color teamColor = std::any_cast<TeamColor::Color>(statlist[WormInfo::TeamColor]);

			hpPosition.y += hpOffset;
			hpPosition.z = ZOrder::z_Checker + additionalZOffset;
			Gear::Renderer2D::DrawQuad(hpPosition, glm::vec2(denoteData.HpTextureWidthUnit, denoteData.HpTextureHeightUnit), denoteData.HpBorder);

			hpPosition.x += 0.15f;
			hpPosition.z = ZOrder::z_Font + additionalZOffset;

			//Name
			std::string name = std::any_cast<std::string>(statlist[WormInfo::Stat::Name]);
			float nameOffset = std::any_cast<float>(statlist[WormInfo::Stat::NameBorderOffset]);
			int strlen = name.length();
			glm::vec3 namePosition = originPosition;

			namePosition.y += nameOffset;
			namePosition.z = ZOrder::z_Checker + additionalZOffset;
			Gear::Renderer2D::DrawQuad(namePosition, glm::vec2(denoteData.NameTextureWidthUnit * strlen * 0.16f, denoteData.NameTextureHeightUnit), denoteData.NameBorder);

			namePosition.x += 0.15f;
			namePosition.y += 0.04f;
			namePosition.z = ZOrder::z_Font + additionalZOffset;
			
			//FlowingArrow
			glm::mat4 arrowTranslate = glm::translate(glm::mat4(1.0f), glm::vec3(originPosition.x, originPosition.y + hpOffset + 1.8f, ZOrder::z_FollowArrow)) *
				glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.4f, 1.0f));
			
			{
				switch (teamColor)
				{
				case InGame::TeamColor::Red:
				{
					auto redArrow = Gear::TextureStorage::GetAnimation2D("RedFollowArrow");
					redArrow->Resume();
					redArrow->Update(timer->GetTick());
					Gear::Renderer2D::DrawAnimation(arrowTranslate, redArrow);
				}
					break;
				case InGame::TeamColor::Blue:
				{
					auto blueArrow = Gear::TextureStorage::GetAnimation2D("BlueFollowArrow");
					blueArrow->Resume();
					blueArrow->Update(timer->GetTick());
					Gear::Renderer2D::DrawAnimation(arrowTranslate, blueArrow);
					Gear::Renderer2D::DrawAnimation(arrowTranslate, blueArrow);
				}
					break;
				case InGame::TeamColor::Green:
				{
					auto blueArrow = Gear::TextureStorage::GetAnimation2D("GreenFollowArrow");
					blueArrow->Resume();
					blueArrow->Update(timer->GetTick());
					Gear::Renderer2D::DrawAnimation(arrowTranslate, blueArrow);
					Gear::Renderer2D::DrawAnimation(arrowTranslate, blueArrow);
				}
				break;
				case InGame::TeamColor::Yellow:
				{
					auto blueArrow = Gear::TextureStorage::GetAnimation2D("YellowFollowArrow");
					blueArrow->Resume();
					blueArrow->Update(timer->GetTick());
					Gear::Renderer2D::DrawAnimation(arrowTranslate, blueArrow);
					Gear::Renderer2D::DrawAnimation(arrowTranslate, blueArrow);
				}
				break;
				case InGame::TeamColor::Purple:
				{
					auto blueArrow = Gear::TextureStorage::GetAnimation2D("PurpleFollowArrow");
					blueArrow->Resume();
					blueArrow->Update(timer->GetTick());
					Gear::Renderer2D::DrawAnimation(arrowTranslate, blueArrow);
					Gear::Renderer2D::DrawAnimation(arrowTranslate, blueArrow);
				}
				break;
				case InGame::TeamColor::Sky:
				{
					auto blueArrow = Gear::TextureStorage::GetAnimation2D("SkyFollowArrow");
					blueArrow->Resume();
					blueArrow->Update(timer->GetTick());
					Gear::Renderer2D::DrawAnimation(arrowTranslate, blueArrow);
					Gear::Renderer2D::DrawAnimation(arrowTranslate, blueArrow);
				}
				break;
				}
			}

			if (onWhite)
			{
				Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::WhiteSmall, 0.3f, false);
				Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::WhiteSmall, 0.3f, false);
			}
			else
			{
				switch (teamColor)
				{
				case InGame::TeamColor::Red:
					Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::RedSmall, 0.3f, false);
					Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::RedSmall, 0.3f, false);
					break;
				case InGame::TeamColor::Blue:
					Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::BlueSmall, 0.3f, false);
					Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::BlueSmall, 0.3f, false);
					break;
				case InGame::TeamColor::Green:
					Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::GreenSmall, 0.3f, false);
					Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::GreenSmall, 0.3f, false);
					break;
				case InGame::TeamColor::Yellow:
					Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::YellowSmall, 0.3f, false);
					Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::YellowSmall, 0.3f, false);
					break;
				case InGame::TeamColor::Purple:
					Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::PurpleSmall, 0.3f, false);
					Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::PurpleSmall, 0.3f, false);
					break;
				case InGame::TeamColor::Sky:
					Font::PrintFont(hpPosition, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::SkySmall, 0.3f, false);
					Font::PrintFont(namePosition, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::SkySmall, 0.3f, false);
					break;
				}
			}
		}
	};

	class WormChangeDisplayPosHanlder : public Gear::Status::StatusHandler
	{
	public:

		void init(int entityID, Gear::Status::StatHandleData& data)
		{
			auto moveData = std::any_cast<std::pair<float, float>>(data.Data);
			moveDist = moveData.first;
			FirstDelay = moveData.second;

			timer = Gear::EntitySystem::GetTimer(entityID);
		}

	private:

		bool inFirst = true;
		const float moveLimit = 10.0f;

		const float BottomNameOffset = 1.36f;
		const float TopNameOffset = 11.26f;
		const float offsetDist = 0.56f;	//within NameBorder and HpBorder

		float pastMove = 0.0f;
		float moveDist;
		float FirstDelay;
		float pastTime = 0.0f;


		Gear::Ref<Gear::Timer> timer;

	private:
		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			float hpOffset = std::any_cast<float>(statlist[WormInfo::HpBorderOffset]);
			float nameOffset = std::any_cast<float>(statlist[WormInfo::NameBorderOffset]);

			if (inFirst)
			{
				init(entityID, data);
			}
			
			if (pastTime < FirstDelay)
			{
				auto tick = timer->GetTick();
				pastTime += tick;
				return;
			}

			auto status = Gear::EntitySystem::GetStatus(entityID);
			status->SetNeedHandleData(WormStatusHandleType::Display, false);

			if (moveDist < 0.0f)
			{
				Gear::EntitySystem::GetStatus(entityID)->SetNeedHandleData(WormStatusHandleType::WaitingDisplay, true);
				if (hpOffset > BottomNameOffset)
				{
					hpOffset += moveDist;
					nameOffset += moveDist;
				}
				else
				{
					GR_TRACE("DisplayPosChage complete");

					hpOffset = BottomNameOffset - offsetDist;
					nameOffset = BottomNameOffset;
					data.Handled = true;

					auto status = Gear::EntitySystem::GetStatus(entityID);
					
				}
			}
			else
			{
				if (hpOffset < TopNameOffset)
				{
					hpOffset += moveDist;
					nameOffset += moveDist;
				}
				else
				{
					GR_TRACE("DisplayPosChage complete");

					hpOffset = TopNameOffset - offsetDist;
					nameOffset = TopNameOffset;
					data.Handled = true;

					status->SetNeedHandleData(WormStatusHandleType::WaitingDisplay, true);
					status->SetNeedHandleData(WormStatusHandleType::Display, true);
				}
			}

			statlist[WormInfo::HpBorderOffset] = hpOffset;
			statlist[WormInfo::NameBorderOffset] = nameOffset;

		}
	};	

	class WormDisplayAimHandler : public Gear::Status::StatusHandler
	{
		float Radius = 2.8f;
		Gear::Ref<Gear::FrameTexture2D> AimTexture;
		bool inFirst = true;

		inline void init(int entityID)
		{
			inFirst = false;
			auto status = Gear::EntitySystem::GetStatus(entityID);
			auto teamColor = std::any_cast<TeamColor::Color>(status->GetStat(WormInfo::TeamColor));

			switch (teamColor)
			{
			case InGame::TeamColor::Red:
				AimTexture = Gear::TextureStorage::GetFrameTexture2D("RedAim");
				break;
			case InGame::TeamColor::Blue:
				AimTexture = Gear::TextureStorage::GetFrameTexture2D("BlueAim");
				break;
			case InGame::TeamColor::Green:
				AimTexture = Gear::TextureStorage::GetFrameTexture2D("GreenAim");
				break;
			case InGame::TeamColor::Yellow:
				AimTexture = Gear::TextureStorage::GetFrameTexture2D("YellowAim");
				break;
			case InGame::TeamColor::Purple:
				AimTexture = Gear::TextureStorage::GetFrameTexture2D("PurpleAim");
				break;
			case InGame::TeamColor::Sky:
				AimTexture = Gear::TextureStorage::GetFrameTexture2D("SkyAim");
				break;
			}

		}

		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			Gear::Ref<Gear::Texture2D> m_Aim;

			if (inFirst)
			{
				init(entityID);
			}
			float nativeFireAngle = std::any_cast<float>(status->GetStat(WormInfo::FireAngle));
			auto WormPosition = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();
			auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));

			float theta;

			if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
			{
				theta = 270.0f - nativeFireAngle / 31.0f * 180.0f;
			}
			else
			{
				theta = (nativeFireAngle - 15.5f) / 15.5f * 90.0f;
			}
			glm::vec3 aimPos(WormPosition.x + Radius * glm::cos(glm::radians(theta)), WormPosition.y + Radius * glm::sin(glm::radians(theta)), ZOrder::z_Aim);
			glm::mat4 aimTrans = glm::translate(glm::mat4(1.0f), aimPos) * glm::scale(glm::mat4(1.0f), glm::vec3(1.9f, 1.9f, 1.0f));

			if (dir == WormInfo::DirectionType::LeftDown || dir == WormInfo::DirectionType::LeftFlat || dir == WormInfo::DirectionType::LeftUp)
			{
				Gear::Renderer2D::DrawFrameQuad(aimTrans, AimTexture, 0, 31 - int(nativeFireAngle));
			}
			else
			{
				Gear::Renderer2D::DrawFrameQuad(aimTrans, AimTexture, 0, int(nativeFireAngle));
			}
		}
	};

	class WormGetDamageHanlder : public Gear::Status::StatusHandler
	{
		bool inFirst = true;

		float damageDisplayDelay = 1.0f;
		float pastTime = 0.0f;

		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			if (inFirst)
			{
				GR_TRACE("Worm Enter WormGetDamageHanlder {0}", Gear::EntitySystem::GetEntity(entityID)->GetName());
				inFirst = false;
				pastTime = 0.0f;
			}
			auto timer = Gear::EntitySystem::GetTimer(entityID);
			pastTime += timer->GetTick();

			if (damageDisplayDelay < pastTime)
			{
				auto FSM = Gear::EntitySystem::GetFSM(entityID);
				FSM->SetCurrentState(WormState::OnNothing);
				inFirst = true;
				data.Paused = true;

				int totalDamage = std::any_cast<int>(status->GetStat(WormInfo::Damage)) + std::any_cast<int>(status->GetStat(WormInfo::SelfDamage));
				status->RegisterPushNeedHandleData(WormStatusHandleType::DisplayDamage, Gear::Status::StatHandleData(0));
			}
		}
	};

	class WormSkipGoHanlder : public Gear::Status::StatusHandler
	{
		bool OnAwake = true;
		bool inFirst = true;
		bool aniChanged = false;
		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::Status> timerStatus;

		float pastTime = 0.0f;
		float TurnOverDelay = 3.0f;

		void Awake(int entityID)
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			FSM = Gear::EntitySystem::GetFSM(entityID);
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);

			int timerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
			timerStatus = Gear::EntitySystem::GetStatus(timerID);
		}

		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			if (inFirst)
			{
				status->RegisterPushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 1.0f)));
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TeamInfoBlinkOff)));
				timerStatus->PushNeedHandleData(1u, Gear::Status::StatHandleData(0));
				inFirst = false;
			}

			pastTime += timer->GetTick();

			if (pastTime > TurnOverDelay)
			{
				inFirst = true;
				aniChanged = false;
				data.Handled = true;
				pastTime = 0.0f;
				timer->SetTimer(1.0f);
				FSM->SetCurrentState(WormState::OnNotMyTurn);

				status->SetStat(WormInfo::MyTurn, false);
				if (GameMode::Bit::ModeBit == GameMode::NetWork)
				{
					Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::NetController });
				}
				else
				{
					Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
				}

				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
				return;
			}

			if (animator->GetFrameIdx().second == 6 && !aniChanged)
			{
				WormInfo::DirectionType dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Stat::Direction));
				switch (dir)
				{
				case WormInfo::DirectionType::LeftFlat:
					animator->SetCurrentAnimation(WormState::OnLeftFlatBreath);
					break;
				case WormInfo::DirectionType::LeftUp:
					animator->SetCurrentAnimation(WormState::OnLeftUpBreath);
					break;
				case WormInfo::DirectionType::LeftDown:
					animator->SetCurrentAnimation(WormState::OnLeftDownBreath);
					break;
				case WormInfo::DirectionType::RightFlat:
					animator->SetCurrentAnimation(WormState::OnRightFlatBreath);
					break;
				case WormInfo::DirectionType::RightUp:
					animator->SetCurrentAnimation(WormState::OnRightUpBreath);
					break;
				case WormInfo::DirectionType::RightDown:
					animator->SetCurrentAnimation(WormState::OnRightDownBreath);
					break;
				}
				animator->ResumeAnimation();
				aniChanged = true;
			}
		}
	};

	class WormDisplayDamageHanlder : public Gear::Status::StatusHandler
	{
		bool firstIn = true;
		int totalDamage = 0;
		int wormGetDamage = 0;
		std::string totalDamageStr;
		int totalDamageLength;

		bool damageDisplayEnd = false;

		float damageBorderOffset = 2.0f;
		glm::vec3 damageBorderPosition;
		Gear::Ref<Gear::Texture2D> damageBorder;
		float damageBorderWidthUnit;
		float damageBorderHeightUnit;

		float accumulateDamage = 0.0f;
		float originHP;

		unsigned int curState;

		int mode;
		//float partitialDamage;
		FontType::Type fontType;

		glm::mat4 damageBorderTrans;

		void init(int entityID)
		{
			firstIn = false;
			auto status = Gear::EntitySystem::GetStatus(entityID);
			originHP = (float)std::any_cast<int>(status->GetStat(WormInfo::Hp));

			totalDamage = std::any_cast<int>(status->GetStat(WormInfo::Damage)) + std::any_cast<int>(status->GetStat(WormInfo::SelfDamage));
			totalDamageStr = std::to_string(totalDamage);
			totalDamageLength = totalDamageStr.length();

			wormGetDamage = totalDamage;
			if (wormGetDamage > originHP)
			{
				wormGetDamage = originHP;
			}

			accumulateDamage = 0.0f;

			damageBorderPosition = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();
			damageBorderPosition.z = ZOrder::z_Font;
			damageBorderPosition.y += damageBorderOffset;

			damageBorder = Gear::TextureStorage::GetTexture2D("WormHPBorder");
			float hpBorderWidth = damageBorder->GetWidth();
			float hpBorderHeight = damageBorder->GetHeight();
			damageBorderWidthUnit = hpBorderWidth / (37.0f * 3) * totalDamageLength / 3;
			damageBorderHeightUnit = hpBorderHeight / (37.0f * 3);

			damageBorderTrans = glm::translate(glm::mat4(1.0f), damageBorderPosition) * glm::scale(glm::mat4(1.0f), glm::vec3(damageBorderWidthUnit, damageBorderHeightUnit, 1.0f));
			damageBorderPosition.x += 0.15f;
			damageBorderTrans[3].z = ZOrder::z_CheckerImg;
		
			curState = std::any_cast<unsigned int>(Gear::EntitySystem::GetFSM(entityID)->GetCurrentState());

			auto teamColor = std::any_cast<TeamColor::Color>(status->GetStat(WormInfo::TeamColor));
			switch (teamColor)
			{
			case TeamColor::Red:
				fontType = FontType::RedSmall;
				break;
			case TeamColor::Blue:
				fontType = FontType::BlueSmall;
				break;
			case TeamColor::Green:
				fontType = FontType::GreenSmall;
				break;
			case TeamColor::Yellow:
				fontType = FontType::YellowSmall;
				break;
			case TeamColor::Purple:
				fontType = FontType::PurpleSmall;
				break;
			case TeamColor::Sky:
				fontType = FontType::SkySmall;
				break;
			}
		}

		int count = 0;
		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			auto status = Gear::EntitySystem::GetStatus(entityID);
			
			if (firstIn)
			{
				mode = std::any_cast<int>(data.Data);
				init(entityID);
			}
			damageBorderOffset += 0.03f;

			damageBorderPosition.y += 0.03f;
			damageBorderTrans[3].y += 0.03f;

			if (totalDamage != 0)
			{
				if (mode)
				{
					Gear::Renderer2D::DrawQuad(damageBorderTrans, damageBorder, glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
					Font::PrintFont(damageBorderPosition, glm::vec3(0.5f, 0.5f, 1.0f), totalDamageStr, fontType, 0.3f, false, glm::vec4(1.0f, 1.0f, 1.0f, 0.3f));
				}
				else
				{
					Gear::Renderer2D::DrawQuad(damageBorderTrans, damageBorder);
					Font::PrintFont(damageBorderPosition, glm::vec3(0.5f, 0.5f, 1.0f), totalDamageStr, fontType, 0.3f, false);
				}
			}

			accumulateDamage += wormGetDamage / 50.0f;
			if (accumulateDamage > wormGetDamage)
			{
				accumulateDamage = wormGetDamage;
			}
			status->SetStat(WormInfo::Hp, int(originHP - accumulateDamage));

			if (damageBorderOffset > 3.5f)
			{
				damageDisplayEnd = true;
			}
			
			if (damageDisplayEnd)
			{
				if (!IS_PLAYING_SOUND(WormsSound::wormSpeech))
				{
					if (std::any_cast<int>(status->GetStat(WormInfo::Damage)) && totalDamage != 0)
					{
						switch (Gear::Util::GetRndInt(5))
						{
						case 0:
							PLAY_SOUND_NAME("YOULLREGRETTHAT", WormsSound::wormSpeech);
								break;
						case 1:
							PLAY_SOUND_NAME("REVENGE", WormsSound::wormSpeech);
								break;
						case 2:
							PLAY_SOUND_NAME("EXCELLENT", WormsSound::wormSpeech);
							break;
						case 3:
							PLAY_SOUND_NAME("OOPS", WormsSound::wormSpeech);
							break;
						case 4:
							PLAY_SOUND_NAME("TRAITOR", WormsSound::wormSpeech);
							break;
						}
					}
					else if(curState != WormState::OnUnderWater)
					{
						PLAY_SOUND_NAME("STUPID", WormsSound::wormSpeech);
					}
				}

				status->SetStat(WormInfo::Damage, 0);
				status->SetStat(WormInfo::SelfDamage, 0);

				damageDisplayEnd = false;
				firstIn = true;
				totalDamage = 0;
				damageBorderOffset = 2.0f;

				auto FSM = Gear::EntitySystem::GetFSM(entityID);

				if (!mode)
				{
					FSM->SetCurrentState(WormState::OnAfterDamaged);
				}
				data.Handled = true;
			}
		}
	};

	class WormAfterUseItemHanlder : public Gear::Status::StatusHandler
	{
		bool inFirst = true;
		bool OnAwake = true;
		bool turnOffController = true;

		float delay;
		float afterDelay;
		float pastTime = 0.0f;
		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::Animator2D> animator;

		void Awake(int entityID)
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			FSM = Gear::EntitySystem::GetFSM(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			OnAwake = false;
		}
		
		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}
			if (inFirst)
			{
				auto delays = std::any_cast<std::pair<float, float>>(data.Data);
				delay = delays.first;
				afterDelay = delays.second;
				pastTime = 0.0f;
				inFirst = false;
			}

			if (pastTime > delay)
			{
				auto curState = FSM->GetCurrentState();
				if (turnOffController)
				{
					if (GameMode::Bit::ModeBit == GameMode::NetWork)
					{
						Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::NetController });
					}
					else
					{
						Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
					}
					if (curState == WormState::OnMove)
					{
						Gear::EntitySystem::GetPhysics2D(entityID)->SetExternalVectorX(0.0f);
						auto dir = std::any_cast<WormInfo::DirectionType>(status->GetStat(WormInfo::Direction));
						switch (dir)
						{
						case InGame::WormInfo::LeftFlat:
							animator->PlayAnimation(WormState::OnLeftFlatBreath);
							break;
						case InGame::WormInfo::RightFlat:
							animator->PlayAnimation(WormState::OnRightFlatBreath);
							break;
						case InGame::WormInfo::LeftUp:
							animator->PlayAnimation(WormState::OnLeftUpBreath);
							break;
						case InGame::WormInfo::RightUp:
							animator->PlayAnimation(WormState::OnRightUpBreath);
							break;
						case InGame::WormInfo::LeftDown:
							animator->PlayAnimation(WormState::OnLeftDownBreath);
							break;
						case InGame::WormInfo::RightDown:
							animator->PlayAnimation(WormState::OnRightDownBreath);
							break;
						}
					}
					turnOffController = false;
				}
			}

			float tick = timer->GetTick();
			pastTime += tick;
			
			if (pastTime > afterDelay)
			{
				if (FSM->GetCurrentState() == WormState::OnBreath)
				{
					FSM->SetCurrentState(WormState::OnAfterUseItem);
					turnOffController = true;
					data.Handled = true;
					inFirst = true;
					return;
				}
				else
				{
					turnOffController = true;
					data.Handled = true;
					inFirst = true;
					return;
				}
			}
		}
	};

	class WormSurrenderHanlder : public Gear::Status::StatusHandler
	{
		bool OnAwake = true;
		bool inFirst = true;
		bool SendSurrenderEvent = false;

		Gear::Ref<Gear::Timer> timer;
		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Animator2D> animator;
		Gear::Ref<Gear::Status> status;
		Gear::Ref<Gear::Status> timerStatus;

		float pastTime = 0.0f;
		float TurnOverDelay = 3.0f;
		float teamSurrenderDelay = 2.0f;

		void Awake(int entityID)
		{
			timer = Gear::EntitySystem::GetTimer(entityID);
			FSM = Gear::EntitySystem::GetFSM(entityID);
			animator = Gear::EntitySystem::GetAnimator2D(entityID);
			status = Gear::EntitySystem::GetStatus(entityID);

			int timerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
			timerStatus = Gear::EntitySystem::GetStatus(timerID);
		}

		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			if (OnAwake)
			{
				Awake(entityID);
			}

			if (inFirst)
			{
				status->RegisterPushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 1.0f)));
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::TeamInfoBlinkOff)));
				timerStatus->PushNeedHandleData(1u, Gear::Status::StatHandleData(0));
				inFirst = false;
				SendSurrenderEvent = false;
			}

			pastTime += timer->GetTick();

			if (pastTime > teamSurrenderDelay && !SendSurrenderEvent)
			{
				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::Surrender, 0, entityID)));
				SendSurrenderEvent = true;
			}

			if (pastTime > TurnOverDelay)
			{
				inFirst = true;
				data.Handled = true;
				pastTime = 0.0f;
				timer->SetTimer(1.0f);
				FSM->SetCurrentState(WormState::OnNotMyTurn);
				status->SetStat(WormInfo::Surrendered, true);

				status->SetStat(WormInfo::MyTurn, false);
				if (GameMode::Bit::ModeBit == GameMode::NetWork)
				{
					Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::NetController });
				}
				else
				{
					Gear::EntitySystem::InActivateComponent(entityID, { Gear::ComponentID::Controller });
				}

				Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
				return;
			}
		}
	};

	class WormTeleportHandler : public Gear::Status::StatusHandler
	{
		Gear::Ref<Gear::FSM> FSM;
		Gear::Ref<Gear::Status> Status;
		Gear::Ref<Gear::Animator2D> Animator;
		Gear::Ref<Gear::Timer> timer;

		bool awake = false;
		
		bool canTurnChange = false;
		float TurnChangeDelay = 2.5f;
		float pastTime = 0.0f;

		void Awake(int entityID)
		{
			FSM = Gear::EntitySystem::GetFSM(entityID);
			Status = Gear::EntitySystem::GetStatus(entityID);
			Animator = Gear::EntitySystem::GetAnimator2D(entityID);
			timer = Gear::EntitySystem::GetTimer(entityID);
		}

		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			if (!awake)
			{
				Awake(entityID);
			}

			unsigned int curAni = Animator->GetCurrentAnimationEnum();
			if (WormState::OnLeftFlatBreath <= curAni && curAni <= WormState::OnRightDownBreath)
			{
				if (canTurnChange)
				{
					pastTime += timer->GetTick();
					if (pastTime > TurnChangeDelay)
					{
						Gear::EventSystem::DispatchEvent(EventChannel::World, Gear::EntityEvent(EventType::World, WorldData(WorldDataType::NewStart)));
						pastTime = 0.0f;
						data.Handled = true;
						canTurnChange = false;
					}
				}
				else
				{
					if (std::any_cast<int>(Status->GetStat(WormInfo::Damage)) + std::any_cast<int>(Status->GetStat(WormInfo::SelfDamage)) == 0)
					{
						Status->RegisterPushNeedHandleData(WormStatusHandleType::DisplayPosChange, Gear::Status::StatHandleData(std::make_pair(-0.2f, 1.0f)));
						FSM->SetCurrentState(WormState::OnNotMyTurn);
						Status->SetStat(WormInfo::MyTurn, false);

						canTurnChange = true;
					}
					else
					{
						data.Handled = true;
					}
				}
			}
		}
	};
}