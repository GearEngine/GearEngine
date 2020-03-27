#pragma once

#include "WormFSMHandler.h"

namespace InGame {

	struct WormDenoteData
	{
		WormDenoteData(float textureWidthUnint, float textureHeightUnit, const Gear::Ref<Gear::Texture2D>& border)
			: TextureWidthUnit(textureWidthUnint), TextureHeightUnit(textureHeightUnit), Border(border)
		{}

		float TextureWidthUnit;
		float TextureHeightUnit;
		Gear::Ref<Gear::Texture2D> Border;
	};

	class WormDisplayNameHanlder : public Gear::Status::StatusHandler
	{
		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			auto& denoteData = std::any_cast<WormDenoteData>(data.Data);
			auto position = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();

			std::string name = std::any_cast<std::string>(statlist[WormInfo::Stat::Name]);
			float offset = std::any_cast<float>(statlist[WormInfo::Stat::NameBorderOffset]);
			float additionalZOffset = std::any_cast<float>(statlist[WormInfo::Stat::ZRenderOffset]);
			int strlen = name.length();

			position.y += offset;
			position.z = ZOrder::z_Checker + additionalZOffset;

			Gear::Renderer2D::DrawQuad(position, glm::vec2(denoteData.TextureWidthUnit * strlen * 0.16f, denoteData.TextureHeightUnit), denoteData.Border);

			position.x += 0.15f;
			position.y += 0.04f;
			position.z = ZOrder::z_Font + additionalZOffset;

			WormInfo::ETeamColor teamColor = std::any_cast<WormInfo::ETeamColor>(statlist[WormInfo::TeamColor]);
			switch (teamColor)
			{
			case InGame::WormInfo::Red:
				Font::PrintFont(position, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::RedSmall, 0.3f, false);
				break;
			case InGame::WormInfo::Blue:
				Font::PrintFont(position, glm::vec3(0.5f, 0.5f, 1.0f), name, FontType::BlueSmall, 0.3f, false);
				break;
			}
		}
	};

	class WormDisplayHpHanlder : public Gear::Status::StatusHandler
	{
		virtual void Handle(int entityID, Gear::Status::StatHandleData& data, std::unordered_map<Gear::EnumType, std::any>& statlist) override
		{
			auto& denoteData = std::any_cast<WormDenoteData>(data.Data);
			auto position = Gear::EntitySystem::GetTransform2D(entityID)->GetPosition();

			int hp = std::any_cast<int>(statlist[WormInfo::Stat::Hp]);
			float offset = std::any_cast<float>(statlist[WormInfo::Stat::HpBorderOffset]);
			float additionalZOffset = std::any_cast<float>(statlist[WormInfo::Stat::ZRenderOffset]);

			position.y += offset;
			position.z = ZOrder::z_Checker + additionalZOffset;

			Gear::Renderer2D::DrawQuad(position, glm::vec2(denoteData.TextureWidthUnit, denoteData.TextureHeightUnit), denoteData.Border);

			position.x += 0.15f;
			position.y += 0.04f;
			position.z = ZOrder::z_Font + additionalZOffset;

			WormInfo::ETeamColor teamColor = std::any_cast<WormInfo::ETeamColor>(statlist[WormInfo::TeamColor]);
			switch (teamColor)
			{
			case InGame::WormInfo::Red:
				Font::PrintFont(position, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::RedSmall, 0.3f, false);
				break;
			case InGame::WormInfo::Blue:
				Font::PrintFont(position, glm::vec3(0.5f, 0.5f, 1.0f), std::to_string(hp), FontType::BlueSmall, 0.3f, false);
				break;
			}
		}
	};

}