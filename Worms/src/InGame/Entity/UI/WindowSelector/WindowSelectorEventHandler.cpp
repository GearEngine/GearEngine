#include "wmpch.h"
#include "WindowSelectorEventHandler.h"

#include "InGame/Entity/UI/Mouse/MouseFSMHandler.h"
#include "InGame/Entity/Object/Worm/WormEnum.h"

namespace InGame {

	void WindowSelectorWormEventHandler::Handle(std::any data, int entityID, bool & handled)
	{
		if (isAwake)
		{
			Awake(entityID);
		}

		auto mapSelectData = std::any_cast<WindowSelectData>(data);
		Status->SetStat(WindowSelectorStat::FromID, mapSelectData.FromID);
		Status->SetStat(WindowSelectorStat::ItemNum, mapSelectData.ItemNum);

		if (mapSelectData.Type == WindowSelctorType::Drop)
		{
			auto& mousePos = GetWMousePos();
			mousePos.first = 0.0f;
			mousePos.second = 0.0f;

			int MouseID = Gear::EntitySystem::GetEntityIDFromName("Mouse");
			Gear::EntitySystem::GetFSM(MouseID)->SetCurrentState(4);
			FSM->SetCurrentState(WindowSelectorState::OnDropSelect);
		}
		else if (mapSelectData.Type == WindowSelctorType::Util)
		{
			auto& mousePos = GetWMousePos();
			mousePos.first = 0.0f;
			mousePos.second = 0.0f;

			int MouseID = Gear::EntitySystem::GetEntityIDFromName("Mouse");
			Gear::EntitySystem::GetFSM(MouseID)->SetCurrentState(4);

			FSM->SetCurrentState(WindowSelectorState::OnUtilSelect);
		}
		if (mapSelectData.Type == WindowSelctorType::Stop)
		{
			FSM->SetCurrentState(WindowSelectorState::OnNothing);
		}
		handled = true;
	}

	bool WindowSelectorMouseEventHandler::isInTerrain(glm::vec2 & worldPosition, int fromID)
	{
		glm::vec3 target(255.0f, 255.0f, 255.0f);
		auto maskTransform = Gear::EntitySystem::GetPhysics2D(fromID)->GetPixelCollisionTargetTextureTranslate();

		auto textureLocalPosition = Gear::Coord2DManger::Get()->GetTextureLocalPosition_From_WorlPosition(worldPosition, maskTransform);

		float texturePositionX = textureLocalPosition.x * mask->GetWidth();
		float texturePositionY = textureLocalPosition.y * mask->GetHeight();

		glm::vec3 pixel;

		for (int i = 0; i < 5; ++i)
		{
			pixel = Gear::Coord2DManger::Get()->GetPixel_From_TextureLocal_With_TextureRealPosition(mask, { texturePositionX + dx[i], texturePositionY + dy[i] });
			if (pixel == target)
			{
				return true;
			}
		}
		return false;
	}

	void WindowSelectorMouseEventHandler::Handle(std::any data, int entityID, bool & handled)
	{
		if (!awake)
		{
			Awake(entityID);
		}

		auto state = FSM->GetCurrentState();
		handled = true;
		auto cmd = std::any_cast<int>(data);

		if (cmd == GR_MOUSE_BUTTON_RIGHT)
		{
			return;
		}
		if (state == WindowSelectorState::OnNothing)
		{
			return;
		}

		if (state == WindowSelectorState::OnUtilSelect)
		{
			auto& mousePos = GetWMousePickPoint();

			float x = (mousePos.first / 2 + 0.5f) * Gear::WINDOW_WIDTH;
			float y = (mousePos.second / 2 + 0.5f) * Gear::WINDOW_HEIGHT;
			glm::vec2 mouseScreenPos(x, y);

			auto worldPos = Gear::Coord2DManger::Get()->GetWorldPosition_From_ScreenPosition(mouseScreenPos);

			auto from = std::any_cast<int>(Status->GetStat(WindowSelectorStat::FromID));

			if (isInTerrain(worldPos, from))
			{
				PLAY_SOUND_NAME("WARNINGBEEP", WormsSound::effect);
			}
			else
			{
				PLAY_SOUND_NAME("Communicator", WormsSound::effect);
				Gear::EntitySystem::GetFSM(from)->GetHandler(WormState::OnReadyItemUse)->OnOut(from);
				Gear::EntitySystem::GetStatus(from)->SetStat(WormInfo::WindowPickedPoint, worldPos);
				Gear::EntitySystem::GetFSM(from)->SetCurrentState(WormState::OnUseWindowPickItem);
				FSM->SetCurrentState(WindowSelectorState::OnNothing);

				int timerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
				Gear::EntitySystem::GetStatus(timerID)->PushNeedHandleData(2, Gear::Status::StatHandleData(0));
				auto mouseID = Gear::EntitySystem::GetEntityIDFromName("Mouse");
				Gear::EntitySystem::GetFSM(mouseID)->SetCurrentState(WorldState::OnRunning);
			}
		}
		if (state == WindowSelectorState::OnDropSelect)
		{
			auto& mousePos = GetWMousePickPoint();

			float x = (mousePos.first / 2 + 0.5f) * Gear::WINDOW_WIDTH;
			float y = (mousePos.second / 2 + 0.5f) * Gear::WINDOW_HEIGHT;
			glm::vec2 mouseScreenPos(x, y);

			auto worldPos = Gear::Coord2DManger::Get()->GetWorldPosition_From_ScreenPosition(mouseScreenPos);

			auto from = std::any_cast<int>(Status->GetStat(WindowSelectorStat::FromID));

			PLAY_SOUND_NAME("CursorSelect", WormsSound::effect);
			Gear::EntitySystem::GetFSM(from)->GetHandler(WormState::OnReadyItemUse)->OnOut(from);
			Gear::EntitySystem::GetStatus(from)->SetStat(WormInfo::WindowPickedPoint, worldPos);
			Gear::EntitySystem::GetFSM(from)->SetCurrentState(WormState::OnUseWindowPickItem);
			FSM->SetCurrentState(WindowSelectorState::OnNothing);

			int timerID = Gear::EntitySystem::GetEntityIDFromName("Timer");
			Gear::EntitySystem::GetStatus(timerID)->PushNeedHandleData(2, Gear::Status::StatHandleData(0));
			auto mouseID = Gear::EntitySystem::GetEntityIDFromName("Mouse");
			Gear::EntitySystem::GetFSM(mouseID)->SetCurrentState(WorldState::OnRunning);
		}
	}


}
