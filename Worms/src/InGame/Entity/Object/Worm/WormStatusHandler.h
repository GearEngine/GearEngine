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
			elapsedTime += Gear::EntitySystem::GetTimer(entityID)->GetTick();
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
				}
			}
		}
	};

	class WormChangeDisplayPosHanlder : public Gear::Status::StatusHandler
	{
		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			static const float moveLimit = 10.0f;
			static float pastMove = 0.0f;

			float hpOffset = std::any_cast<float>(statlist[WormInfo::HpBorderOffset]);
			float nameOffset = std::any_cast<float>(statlist[WormInfo::NameBorderOffset]);

			float moveDist = std::any_cast<float>(data.Data);
			hpOffset += moveDist;
			nameOffset += moveDist;
			pastMove += moveDist;

			statlist[WormInfo::HpBorderOffset] = hpOffset;
			statlist[WormInfo::NameBorderOffset] = nameOffset;

			if (std::abs(pastMove) > moveLimit)
			{
				data.Handled = true;
				pastMove = 0.0f;
				Gear::EntitySystem::GetStatus(entityID)->SetNeedHandleData(WormStatusHandleType::WaitingDisplay, true);
			}
		}
	};

}